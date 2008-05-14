#include "gtk-config-store.h"
#include "attribute-iterator.h"
#include "ns3/config.h"
#include "ns3/string.h"
#include <gtk/gtk.h>
#include <fstream>


namespace ns3 {

enum {
  COL_NAME = 0,
  COL_VALUE,
  COL_LAST
};

struct AttributeNode
{
  std::string name;
  std::string path;
  Ptr<Object> object;
};

class ModelCreator : public AttributeIterator
{
public:
  ModelCreator ();

  void Build (GtkTreeStore *treestore);
private:
  virtual void DoVisit (Ptr<Object> object, std::string name, std::string path);
  virtual void DoPush (std::string name, std::string path);
  virtual void DoPop (void);

  GtkTreeStore *m_treestore;
  std::vector<GtkTreeIter *> m_iters;
};

ModelCreator::ModelCreator ()
{}
void 
ModelCreator::Build (GtkTreeStore *treestore)
{
  m_treestore = treestore;
  m_iters.push_back (NULL);
  Iterate ();
  NS_ASSERT (m_iters.size () == 1);
}
void 
ModelCreator::DoVisit (Ptr<Object> object, std::string name, std::string path)
{
  StringValue str;
  object->GetAttribute (name, str);
  GtkTreeIter *parent = m_iters.back ();
  GtkTreeIter current;
  gtk_tree_store_append (m_treestore, &current, parent);
  AttributeNode *node = new AttributeNode ();
  node->name = name;
  node->path = path + "/" + str.Get ();
  node->object = object;
  gtk_tree_store_set (m_treestore, &current,
		      COL_NAME, g_strdup (name.c_str ()), 
		      COL_VALUE, node,
		      -1);
}
void 
ModelCreator::DoPush (std::string name, std::string path)
{
  GtkTreeIter *parent = m_iters.back ();
  GtkTreeIter *current = g_new (GtkTreeIter, 1);
  gtk_tree_store_append (m_treestore, current, parent);
  gtk_tree_store_set (m_treestore, current,
		      COL_NAME, g_strdup (name.c_str ()),
		      COL_VALUE, NULL, 
                     -1);
  m_iters.push_back (current);
}
void 
ModelCreator::DoPop (void)
{
  GtkTreeIter *current = m_iters.back ();
  g_free (current);
  m_iters.pop_back ();
}

static void
attribute_cell_data_function (GtkTreeViewColumn *col,
			      GtkCellRenderer   *renderer,
			      GtkTreeModel      *model,
			      GtkTreeIter       *iter,
			      gpointer           user_data)
{
  AttributeNode *node = 0;
  gtk_tree_model_get (model, iter, COL_VALUE, &node, -1);
  if (node != 0)
    {
      StringValue str;
      node->object->GetAttribute (node->name, str);
      g_object_set(renderer, "text", str.Get ().c_str (), NULL);
    }
  else
    {
      g_object_set(renderer, "text", "", NULL);
    }
}

static void
cell_edited_callback (GtkCellRendererText *cell,
		      gchar               *path_string,
		      gchar               *new_text,
		      gpointer             user_data)
{
  GtkTreeModel *model = GTK_TREE_MODEL (user_data);
  GtkTreeIter iter;
  gtk_tree_model_get_iter_from_string (model, &iter, path_string);
  AttributeNode *node;
  gtk_tree_model_get (model, &iter, COL_VALUE, &node, -1);
  node->object->SetAttribute (node->name, StringValue (new_text));
}


static GtkWidget *
create_view_and_model (void)
{
  GtkTreeViewColumn   *col;
  GtkCellRenderer     *renderer;
  GtkWidget           *view;

  GtkTreeStore *model = gtk_tree_store_new (COL_LAST, G_TYPE_STRING, G_TYPE_POINTER);
  ModelCreator creator;
  creator.Build (model);


  view = gtk_tree_view_new();
  gtk_tree_view_set_grid_lines (GTK_TREE_VIEW (view), GTK_TREE_VIEW_GRID_LINES_BOTH);

  col = gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(col, "Object Attributes");
  gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);
  renderer = gtk_cell_renderer_text_new ();
  gtk_tree_view_column_pack_start(col, renderer, TRUE);
  gtk_tree_view_column_add_attribute(col, renderer, "text", COL_NAME);

  col = gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(col, "Attribute Value");
  gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);
  renderer = gtk_cell_renderer_text_new();
  g_object_set(renderer, "editable", TRUE, NULL);
  g_signal_connect(renderer, "edited", (GCallback) cell_edited_callback, model);
  gtk_tree_view_column_pack_start(col, renderer, TRUE);
  gtk_tree_view_column_set_cell_data_func(col, renderer, attribute_cell_data_function, NULL, NULL);


  gtk_tree_view_set_model(GTK_TREE_VIEW(view), GTK_TREE_MODEL (model));

  g_object_unref(model); /* destroy model automatically with view */


  return view;
}

static void
save_clicked (GtkButton *button,
	      gpointer   user_data)
{
  GtkWidget *parent_window = GTK_WIDGET (user_data);
  GtkWidget *dialog;

  dialog = gtk_file_chooser_dialog_new ("Save File",
					GTK_WINDOW (parent_window),
					GTK_FILE_CHOOSER_ACTION_SAVE,
					GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
					GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
					NULL);
  gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER (dialog), TRUE);

  gtk_file_chooser_set_current_name (GTK_FILE_CHOOSER (dialog), "config.txt");


  if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
    {
      char *filename;

      filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
      std::ofstream os;
      os.open (filename);
      TextFileAttributeIterator file = TextFileAttributeIterator (os);
      file.Save ();
      os.close ();
      g_free (filename);
    }

  gtk_widget_destroy (dialog);
}

static void
load_clicked (GtkButton *button,
	      gpointer   user_data)
{
  GtkWidget *parent_window = GTK_WIDGET (user_data);
  GtkWidget *dialog;

  dialog = gtk_file_chooser_dialog_new ("Open File",
					GTK_WINDOW (parent_window),
					GTK_FILE_CHOOSER_ACTION_OPEN,
					GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
					GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
					NULL);

  if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
    {
      char *filename;

      filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
      std::ifstream is;
      is.open (filename, std::ios::in);
      std::string path, value;
      while (is.good())
	{
	  is >> path >> value;
	  Config::Set (path, StringValue (value));
	}
      g_free (filename);
    }

  gtk_widget_destroy (dialog);
}

GtkConfigStore::GtkConfigStore ()
{}

void 
GtkConfigStore::Configure (void)
{
  GtkWidget *window;
  GtkWidget *view;

  gtk_init (0, 0);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  g_signal_connect (window, "delete_event", gtk_main_quit, NULL); /* dirty */

  view = create_view_and_model ();

  GtkWidget *vbox = gtk_vbox_new (FALSE, 5);
  gtk_box_pack_start (GTK_BOX (vbox), view, TRUE, TRUE, 0);
  gtk_box_pack_end (GTK_BOX (vbox), gtk_hseparator_new (), FALSE, FALSE, 0);
  GtkWidget *hbox = gtk_hbox_new (FALSE, 5);
  gtk_box_pack_end (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
  GtkWidget *save = gtk_button_new_with_label ("Save");
  g_signal_connect (save, "clicked",  (GCallback) save_clicked, window);
  gtk_box_pack_end (GTK_BOX (hbox), save, FALSE, FALSE, 0);
  GtkWidget *load = gtk_button_new_with_label ("Load");
  g_signal_connect (load, "clicked",  (GCallback) load_clicked, window);
  gtk_box_pack_end (GTK_BOX (hbox), load, FALSE, FALSE, 0);
  GtkWidget *exit = gtk_button_new_with_label ("Exit");
  g_signal_connect (exit, "clicked",  (GCallback) gtk_main_quit, NULL);
  gtk_box_pack_end (GTK_BOX (hbox), exit, FALSE, FALSE, 0);

  gtk_container_add (GTK_CONTAINER (window), vbox);

  gtk_widget_show_all (window);

  gtk_main ();
}

} // namespace ns3
