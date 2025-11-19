#!/usr/bin/env python3
"""
Neural Network-based Link Quality Estimator for VANET AODV Routing
Classifies vehicle links as: Connected (2), Transient (1), Disconnected (0)
"""

import numpy as np
import torch
import torch.nn as nn
import torch.optim as optim
import argparse
import csv
from datetime import datetime
import os
import sys
import time
import warnings

# Suppress gym deprecation warnings
warnings.filterwarnings('ignore', category=DeprecationWarning)

# Try importing ns3gym with better error handling
try:
    from ns3gym import ns3env
    print("✓ ns3gym imported successfully")
except ImportError as e:
    print(f"✗ Error importing ns3gym: {e}")
    print("\nPlease install ns3gym:")
    print("  pip install ns3-gym")
    sys.exit(1)

# Neural Network for Link Quality Classification
class LinkQualityNN(nn.Module):
    def __init__(self, input_size=5, hidden_size=64):
        super(LinkQualityNN, self).__init__()
        self.fc1 = nn.Linear(input_size, hidden_size)
        self.fc2 = nn.Linear(hidden_size, hidden_size)
        self.fc3 = nn.Linear(hidden_size, 32)
        self.fc4 = nn.Linear(32, 3)  # 3 classes: disconnected, transient, connected
        self.relu = nn.ReLU()
        self.dropout = nn.Dropout(0.3)

    def forward(self, x):
        x = self.relu(self.fc1(x))
        x = self.dropout(x)
        x = self.relu(self.fc2(x))
        x = self.dropout(x)
        x = self.relu(self.fc3(x))
        x = self.fc4(x)
        return x

class LinkQualityEstimator:
    def __init__(self, port=5555, collect_data=True):
        self.port = port
        self.collect_data = collect_data

        # Initialize neural network
        self.device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
        self.model = LinkQualityNN().to(self.device)
        self.criterion = nn.CrossEntropyLoss()
        self.optimizer = optim.Adam(self.model.parameters(), lr=0.001)

        # Dataset for collection
        self.dataset = []
        self.dataset_file = f"vanet_link_dataset_{datetime.now().strftime('%Y%m%d_%H%M%S')}.csv"

        # Statistics
        self.total_steps = 0
        self.classifications = {"disconnected": 0, "transient": 0, "connected": 0}
        self.episode_rewards = []
        self.current_episode_reward = 0

        print(f"✓ Link Quality Estimator initialized on {self.device}")
        print(f"✓ Dataset will be saved to: {self.dataset_file}")

    def normalize_observation(self, obs):
        """Normalize observation values - obs should already be normalized from NS-3"""
        # Convert to numpy array with explicit dtype
        obs = np.array(obs, dtype=np.float32)

        # Reshape to (num_links, 5) where 5 = [RSSI, SNR, PacketLoss, Distance, RelSpeed]
        num_metrics = 5
        
        if len(obs) == 0:
            return np.array([], dtype=np.float32).reshape(0, num_metrics)
        
        num_links = len(obs) // num_metrics

        if len(obs) % num_metrics != 0:
            # Pad if necessary
            padding = num_metrics - (len(obs) % num_metrics)
            obs = np.pad(obs, (0, padding), mode='constant', constant_values=0.0)
            num_links = len(obs) // num_metrics

        obs = obs.reshape(num_links, num_metrics)

        # Values should already be normalized in [0, 1] from NS-3
        # Just clip to ensure they're in valid range
        obs = np.clip(obs, 0.0, 1.0)

        return obs

    def classify_link_quality(self, link_metrics):
        """
        Classify link quality using heuristic rules
        3 classes: 0=Disconnected, 1=Transient, 2=Connected
        """
        rssi, snr, packet_loss, distance, rel_speed = link_metrics

        # Rule-based classification for ground truth
        # (Values are normalized to [0, 1])
        if distance > 0.6 or rssi < 0.2 or packet_loss > 0.5:
            return 0  # Disconnected
        elif distance > 0.3 or packet_loss > 0.2 or rel_speed > 0.6:
            return 1  # Transient
        else:
            return 2  # Connected

    def predict_link_quality(self, obs):
        """Predict link quality using neural network"""
        if len(obs) == 0:
            return np.array([], dtype=np.int64)
        
        obs_tensor = torch.FloatTensor(obs).to(self.device)

        with torch.no_grad():
            self.model.eval()
            outputs = self.model(obs_tensor)
            _, predicted = torch.max(outputs, 1)

        return predicted.cpu().numpy()

    def train_step(self, obs, labels):
        """Train the neural network"""
        if len(obs) == 0 or len(labels) == 0:
            return 0.0
        
        obs_tensor = torch.FloatTensor(obs).to(self.device)
        labels_tensor = torch.LongTensor(labels).to(self.device)

        self.model.train()
        self.optimizer.zero_grad()

        outputs = self.model(obs_tensor)
        loss = self.criterion(outputs, labels_tensor)

        loss.backward()
        self.optimizer.step()

        return loss.item()

    def save_to_dataset(self, obs, labels):
        """Save observations and labels to dataset file"""
        if not self.collect_data or len(obs) == 0:
            return

        file_exists = os.path.isfile(self.dataset_file)

        try:
            with open(self.dataset_file, 'a', newline='') as f:
                writer = csv.writer(f)

                # Write header if file is new
                if not file_exists:
                    header = ['RSSI', 'SNR', 'PacketLoss', 'Distance', 'RelSpeed', 'LinkQuality']
                    writer.writerow(header)

                # Write data
                for obs_row, label in zip(obs, labels):
                    row = list(obs_row) + [label]
                    writer.writerow(row)

            self.dataset.append((obs, labels))
        except Exception as e:
            print(f"Warning: Could not save to dataset: {e}")

    def run(self, max_steps=10000):
        """Main agent loop"""
        print(f"\n{'='*60}")
        print("Connecting to ns3-gym environment...")
        print(f"{'='*60}")
        print(f"Port: {self.port}")
        print(f"Step Time: 0.1s")
        print(f"Start Simulation: False (NS-3 should already be running)")
        print(f"{'='*60}\n")

        env = None
        connected = False
        max_retries = 10
        retry_delay = 3

        for attempt in range(1, max_retries + 1):
            try:
                print(f"Connection attempt {attempt}/{max_retries}...")
                
                # Create environment with explicit parameters
                env = ns3env.Ns3Env(
                    port=self.port,
                    stepTime=0.1,
                    startSim=False,
                    simSeed=0,
                    simArgs={},
                    debug=False
                )
                
                print("✓ Connected to NS-3 simulation successfully!")
                connected = True
                break
                
            except Exception as e:
                print(f"✗ Connection failed: {e}")
                if attempt < max_retries:
                    print(f"Retrying in {retry_delay} seconds...")
                    time.sleep(retry_delay)
                else:
                    print("\n" + "="*60)
                    print("Failed to connect after maximum retries")
                    print("="*60)
                    print("\nTroubleshooting:")
                    print("1. Verify NS-3 simulation is running")
                    print("2. Check port number matches (5555)")
                    print("3. Ensure no firewall blocking")
                    print("4. Try restarting both NS-3 and Python")
                    return

        if not connected or env is None:
            print("✗ Could not establish connection to NS-3")
            return

        print("\n" + "="*60)
        print("Starting link quality estimation...")
        print("="*60 + "\n")

        # Reset environment
        try:
            obs = env.reset()
            print(f"✓ Environment reset successful")
            print(f"✓ Initial observation size: {len(obs)}")
        except Exception as e:
            print(f"✗ Error during reset: {e}")
            env.close()
            return

        step = 0
        self.current_episode_reward = 0
        last_print_time = time.time()

        try:
            while step < max_steps:
                # Normalize observation
                obs_normalized = self.normalize_observation(obs)

                if len(obs_normalized) == 0:
                    # No link data yet, use default action
                    if time.time() - last_print_time > 5:
                        print(f"Step {step}: Waiting for link data...")
                        last_print_time = time.time()
                    action = 1  # Default to "transient" class
                else:
                    # Generate ground truth labels using heuristics
                    labels = []
                    for link_obs in obs_normalized:
                        label = self.classify_link_quality(link_obs)
                        labels.append(label)

                        # Update statistics
                        if label == 0:
                            self.classifications["disconnected"] += 1
                        elif label == 1:
                            self.classifications["transient"] += 1
                        else:
                            self.classifications["connected"] += 1

                    # Save to dataset
                    self.save_to_dataset(obs_normalized, labels)

                    # Train neural network periodically
                    if step > 100 and step % 10 == 0:
                        loss = self.train_step(obs_normalized, labels)
                        if step % 100 == 0 and loss > 0:
                            print(f"Step {step}: Training loss = {loss:.4f}")

                    # Use neural network predictions
                    predictions = self.predict_link_quality(obs_normalized)
                    
                    # Use first prediction as action
                    action = int(predictions[0]) if len(predictions) > 0 else 1

                # Execute action in environment
                try:
                    obs, reward, done, info = env.step(action)
                    self.current_episode_reward += reward
                    step += 1
                except Exception as e:
                    print(f"✗ Error during step {step}: {e}")
                    break

                # Print progress
                if step % 100 == 0:
                    total_links = sum(self.classifications.values())
                    if total_links > 0:
                        print(f"\n{'='*60}")
                        print(f"Step {step}/{max_steps} | Reward: {self.current_episode_reward:.2f}")
                        print(f"Link Classifications:")
                        print(f"  Connected:    {self.classifications['connected']:6d} ({100*self.classifications['connected']/total_links:.1f}%)")
                        print(f"  Transient:    {self.classifications['transient']:6d} ({100*self.classifications['transient']/total_links:.1f}%)")
                        print(f"  Disconnected: {self.classifications['disconnected']:6d} ({100*self.classifications['disconnected']/total_links:.1f}%)")
                        print(f"{'='*60}\n")

                if done:
                    print(f"\n✓ Episode finished at step {step}")
                    self.episode_rewards.append(self.current_episode_reward)
                    print(f"Episode reward: {self.current_episode_reward:.2f}")
                    
                    try:
                        obs = env.reset()
                        self.current_episode_reward = 0
                        print("✓ Environment reset for new episode")
                    except Exception as e:
                        print(f"✗ Error resetting environment: {e}")
                        break

        except KeyboardInterrupt:
            print("\n\n⚠ Training interrupted by user")
        except Exception as e:
            print(f"\n\n✗ Error during training: {e}")
            import traceback
            traceback.print_exc()
        finally:
            self.print_final_statistics(step)
            self.save_model()
            if env is not None:
                try:
                    env.close()
                    print("✓ Environment closed successfully")
                except:
                    pass
            print("\nSimulation ended")

    def print_final_statistics(self, steps):
        """Print final statistics"""
        print("\n" + "="*60)
        print("              FINAL STATISTICS")
        print("="*60)
        print(f"Total steps executed: {steps}")
        print(f"Total samples collected: {len(self.dataset)}")
        print(f"\nLink Classifications:")
        total = sum(self.classifications.values())
        if total > 0:
            print(f"  Connected:    {self.classifications['connected']:6d} ({100*self.classifications['connected']/total:.1f}%)")
            print(f"  Transient:    {self.classifications['transient']:6d} ({100*self.classifications['transient']/total:.1f}%)")
            print(f"  Disconnected: {self.classifications['disconnected']:6d} ({100*self.classifications['disconnected']/total:.1f}%)")
        else:
            print("  No classifications recorded")
        
        if len(self.episode_rewards) > 0:
            print(f"\nEpisode Rewards:")
            print(f"  Mean:   {np.mean(self.episode_rewards):.2f}")
            print(f"  Std:    {np.std(self.episode_rewards):.2f}")
            print(f"  Min:    {np.min(self.episode_rewards):.2f}")
            print(f"  Max:    {np.max(self.episode_rewards):.2f}")
        
        print(f"\nDataset saved to: {self.dataset_file}")
        print("="*60)

    def save_model(self):
        """Save the trained model"""
        try:
            model_file = f"link_quality_model_{datetime.now().strftime('%Y%m%d_%H%M%S')}.pth"
            torch.save({
                'model_state_dict': self.model.state_dict(),
                'optimizer_state_dict': self.optimizer.state_dict(),
                'classifications': self.classifications,
                'episode_rewards': self.episode_rewards
            }, model_file)
            print(f"✓ Model saved to: {model_file}")
        except Exception as e:
            print(f"✗ Could not save model: {e}")

def main():
    parser = argparse.ArgumentParser(
        description='VANET Link Quality Estimation Agent',
        formatter_class=argparse.ArgumentDefaultsHelpFormatter
    )
    parser.add_argument('--port', type=int, default=5555, 
                       help='Port for ns3-gym communication')
    parser.add_argument('--steps', type=int, default=10000, 
                       help='Maximum training steps')
    parser.add_argument('--no-collect', action='store_true', 
                       help='Disable dataset collection')

    args = parser.parse_args()

    print("\n" + "="*60)
    print("   VANET Link Quality Estimation with Neural Networks")
    print("="*60)
    print(f"Port:            {args.port}")
    print(f"Max Steps:       {args.steps}")
    print(f"Data Collection: {'Disabled' if args.no_collect else 'Enabled'}")
    print("="*60 + "\n")

    estimator = LinkQualityEstimator(
        port=args.port,
        collect_data=not args.no_collect
    )

    estimator.run(max_steps=args.steps)

if __name__ == "__main__":
    main()