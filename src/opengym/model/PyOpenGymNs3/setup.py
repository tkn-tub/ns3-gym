from setuptools import setup, find_packages


def readme():
    with open('README.md') as f:
        return f.read()


setup(
    name='PyOpenGymNs3',
    version='0.1.0',
    packages=find_packages(),
    scripts=[],
    url='',
    license='MIT',
    author='Piotr Gawlowicz',
    author_email='gawlowicz.p@gmail.com',
    description='OpenAI Gym meets ns-3',
    long_description='OpenAI Gym meets ns-3',
    keywords='openAI gym, ML, RL, ns-3',
    install_requires=['pyzmq', 'numpy', 'protobuf', 'protobuf3'],
    extras_require={},
)
