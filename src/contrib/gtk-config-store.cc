#include "gtk-config-store.h"
#include "attribute-iterator.h"
#include "ns3/config.h"
#include "ns3/string.h"
#include "ns3/pointer.h"
#include <gtk/gtk.h>
#include <fstream>


namespace ns3 {

enum {
  COL_NODE = 0,
  COL_LAST
};

struct ModelNode
{
  enum {
    // store object + attribute name
    NODE_ATTRIBUTE,
    // store object + attribute name
    NODE_POINTER,
    // store object + attribute name
    NODE_VECTOR,
    // store index + value (object)
    NODE_VECTOR_ITEM,
    // store object
    NODE_OBJECT
  } type;
  std::string name;
  Ptr<Object> object;
  uint32_t index;
};

class ModelCreator : public AttributeIterator
{
public:
  ModelCreator ();

  void Build (GtkTreeStore *treestore);
private:
  virtual void DoVisitAttribute (Ptr<Object> object, std::string name);
  virtual void DoStartVisitObject (Ptr<Object> object);
  virtual void DoEndVisitObject (void);
  virtual void DoStartVisitPointerAttribute (Ptr<Object> object, std::string name, Ptr<Object> value);
  virtual void DoEndVisitPointerAttribute (void);
  virtual void DoStartVisitArrayAttribute (Ptr<Object> object, std::string name, const ObjectVectorValue &vector);
  virtual void DoEndVisitArrayAttribute (void);
  virtual void DoStartVisitArrayItem (const ObjectVectorValue &vector, uint32_t index, Ptr<Object> item);
  virtual void DoEndVisitArrayItem (void);

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
ModelCreator::DoVisitAttribute (Ptr<Object> object, std::string name)
{
  GtkTreeIter *parent = m_iters.back ();
  GtkTreeIter current;
  ModelNode *node = new ModelNode ();
  node->type = ModelNode::NODE_ATTRIBUTE;
  node->object = object;
  node->name = name;
  gtk_tree_store_append (m_treestore, &current, parent);
  gtk_tree_store_set (m_treestore, &current,
		      COL_NODE, node,
                     -1);
}
void 
ModelCreator::DoStartVisitObject (Ptr<Object> object)
{
  GtkTreeIter *parent = m_iters.back ();
  GtkTreeIter *current = g_new (GtkTreeIter, 1);
  ModelNode *node = new ModelNode ();
  node->type = ModelNode::NODE_OBJECT;
  node->object = object;
  gtk_tree_store_append (m_treestore, current, parent);
  gtk_tree_store_set (m_treestore, current,
		      COL_NODE, node,
                     -1);
  m_iters.push_back (current);
}
void 
ModelCreator::DoEndVisitObject (void)
{
  GtkTreeIter *iter = m_iters.back ();
  g_free (iter);
  m_iters.pop_back ();
}
void 
ModelCreator::DoStartVisitPointerAttribute (Ptr<Object> object, std::string name, Ptr<Object> value)
{
  GtkTreeIter *parent = m_iters.back ();
  GtkTreeIter *current = g_new (GtkTreeIter, 1);
  ModelNode *node = new ModelNode ();
  node->type = ModelNode::NODE_POINTER;
  node->object = object;
  node->name = name;
  gtk_tree_store_append (m_treestore, current, parent);
  gtk_tree_store_set (m_treestore, current,
		      COL_NODE, node,
                     -1);
  m_iters.push_back (current);
}
void 
ModelCreator::DoEndVisitPointerAttribute (void)
{
  GtkTreeIter *iter = m_iters.back ();
  g_free (iter);
  m_iters.pop_back ();  
}
void 
ModelCreator::DoStartVisitArrayAttribute (Ptr<Object> object, std::string name, const ObjectVectorValue &vector)
{
  GtkTreeIter *parent = m_iters.back ();
  GtkTreeIter *current = g_new (GtkTreeIter, 1);
  ModelNode *node = new ModelNode ();
  node->type = ModelNode::NODE_VECTOR;
  node->object = object;
  node->name = name;
  gtk_tree_store_append (m_treestore, current, parent);
  gtk_tree_store_set (m_treestore, current,
		      COL_NODE, node,
                     -1);
  m_iters.push_back (current);
}
void 
ModelCreator::DoEndVisitArrayAttribute (void)
{
  GtkTreeIter *iter = m_iters.back ();
  g_free (iter);
  m_iters.pop_back ();  
}
void 
ModelCreator::DoStartVisitArrayItem (const ObjectVectorValue &vector, uint32_t index, Ptr<Object> item)
{
  GtkTreeIter *parent = m_iters.back ();
  GtkTreeIter *current = g_new (GtkTreeIter, 1);
  ModelNode *node = new ModelNode ();
  node->type = ModelNode::NODE_VECTOR_ITEM;
  node->object = item;
  node->index = index;
  gtk_tree_store_append (m_treestore, current, parent);
  gtk_tree_store_set (m_treestore, current,
		      COL_NODE, node,
                     -1);
  m_iters.push_back (current);
}
void 
ModelCreator::DoEndVisitArrayItem (void)
{
  GtkTreeIter *iter = m_iters.back ();
  g_free (iter);
  m_iters.pop_back ();  
}

static void
cell_data_function_col_1 (GtkTreeViewColumn *col,
			  GtkCellRenderer   *renderer,
			  GtkTreeModel      *model,
			  GtkTreeIter       *iter,
			  gpointer           user_data)
{
  ModelNode *node;
  gtk_tree_model_get (model, iter, COL_NODE, &node, -1);
  if (node->type == ModelNode::NODE_ATTRIBUTE)
    {
      StringValue str;
      node->object->GetAttribute (node->name, str);
      g_object_set(renderer, "text", str.Get ().c_str (), NULL);
      g_object_set(renderer, "editable", TRUE, NULL);
    }
  else
    {
      g_object_set(renderer, "text", "", NULL);
      g_object_set(renderer, "editable", FALSE, NULL);
    }
}

static void
cell_data_function_col_0 (GtkTreeViewColumn *col,
			  GtkCellRenderer   *renderer,
			  GtkTreeModel      *model,
			  GtkTreeIter       *iter,
			  gpointer           user_data)
{
  ModelNode *node;
  gtk_tree_model_get (model, iter, COL_NODE, &node, -1);
  g_object_set (renderer, "editable", FALSE, NULL);
  switch (node->type) {
  case ModelNode::NODE_OBJECT:
    g_object_set(renderer, "text", node->object->GetInstanceTypeId ().GetName ().c_str (), NULL);
    break;
  case ModelNode::NODE_POINTER:
    g_object_set(renderer, "text", node->name.c_str (), NULL);
    break;
  case ModelNode::NODE_VECTOR:
    g_object_set(renderer, "text", node->name.c_str (), NULL);
    break;
  case ModelNode::NODE_VECTOR_ITEM: {
    std::stringstream oss;
    oss << node->index;
    g_object_set(renderer, "text", oss.str ().c_str (), NULL);
  } break;
  case ModelNode::NODE_ATTRIBUTE:
    g_object_set(renderer, "text", node->name.c_str (), NULL);
    break;
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
  ModelNode *node;
  gtk_tree_model_get (model, &iter, COL_NODE, &node, -1);
  NS_ASSERT (node->type == ModelNode::NODE_ATTRIBUTE);
  node->object->SetAttribute (node->name, StringValue (new_text));
}

static int
get_col_number_from_tree_view_column (GtkTreeViewColumn *col)
{
  GList *cols;
  int   num;
  g_return_val_if_fail ( col != NULL, -1 );
  g_return_val_if_fail ( col->tree_view != NULL, -1 );
  cols = gtk_tree_view_get_columns(GTK_TREE_VIEW(col->tree_view));
  num = g_list_index(cols, (gpointer) col);
  g_list_free(cols);
  return num;
}

static gboolean
cell_tooltip_callback (GtkWidget  *widget,
		       gint        x,
		       gint        y,
		       gboolean    keyboard_tip,
		       GtkTooltip *tooltip,
		       gpointer    user_data)
{
  GtkTreeModel *model;
  GtkTreeIter iter;
  GtkTreeViewColumn * column;
  if (!gtk_tree_view_get_tooltip_context (GTK_TREE_VIEW (widget), 
					  &x, &y, keyboard_tip,
					  &model, NULL, &iter))
    {
      return FALSE;
    }
  if (!gtk_tree_view_get_path_at_pos (GTK_TREE_VIEW (widget),
				      x, y, NULL, &column, NULL, NULL))
    {
      return FALSE;
    }  
  int col = get_col_number_from_tree_view_column (column);

  ModelNode *node;
  gtk_tree_model_get (model, &iter, COL_NODE, &node, -1);

  switch (node->type) {
  case ModelNode::NODE_OBJECT:
    if (col == 0)
      {
	std::string tip = "This object is of type " + node->object->GetInstanceTypeId ().GetName ();
	gtk_tooltip_set_text (tooltip, tip.c_str ());
	return TRUE;
      }
    break;
  case ModelNode::NODE_POINTER:
    if (col == 0)
      {
	PointerValue ptr;
	node->object->GetAttribute (node->name, ptr);
	std::string tip = "This object is of type " + ptr.GetObject ()->GetInstanceTypeId ().GetName ();
	gtk_tooltip_set_text (tooltip, tip.c_str ());
	return TRUE;
      }
    break;
  case ModelNode::NODE_VECTOR:
    break;
  case ModelNode::NODE_VECTOR_ITEM:
    if (col == 0)
      {
	std::string tip = "This object is of type " + node->object->GetInstanceTypeId ().GetName ();
	gtk_tooltip_set_text (tooltip, tip.c_str ());
	return TRUE;
      }
    break;
  case ModelNode::NODE_ATTRIBUTE: {
    TypeId tid = node->object->GetInstanceTypeId ();
    uint32_t attrIndex;
    for (uint32_t i = 0; i < tid.GetAttributeN (); ++i)
      {
	if (tid.GetAttributeName (i) == node->name)
	  {
	    attrIndex = i;
	    break;
	  }
      }
    if (col == 0)
      {
	std::string tip = tid.GetAttributeHelp (attrIndex);
	gtk_tooltip_set_text (tooltip, tip.c_str ());
      }
    else
      {
	Ptr<const AttributeChecker> checker = tid.GetAttributeChecker (attrIndex);
	std::string tip;
	tip = "This attribute is of type " + checker->GetValueTypeName ();
	if (checker->HasUnderlyingTypeInformation ())
	  {
	    tip += " " + checker->GetUnderlyingTypeInformation ();
	  }
	gtk_tooltip_set_text (tooltip, tip.c_str ());
      }
    return TRUE;
  } break;
  }
  return FALSE;
}


static GtkWidget *
create_view_and_model (void)
{
  GtkTreeViewColumn   *col;
  GtkCellRenderer     *renderer;
  GtkWidget           *view;

  GtkTreeStore *model = gtk_tree_store_new (COL_LAST, G_TYPE_POINTER);
  ModelCreator creator;
  creator.Build (model);


  view = gtk_tree_view_new();
  g_object_set (view, "has-tooltip", TRUE, NULL);
  g_signal_connect (view, "query-tooltip", (GCallback) cell_tooltip_callback, NULL);
  
  gtk_tree_view_set_grid_lines (GTK_TREE_VIEW (view), GTK_TREE_VIEW_GRID_LINES_BOTH);
  gtk_tree_view_set_rules_hint (GTK_TREE_VIEW (view), TRUE);

  col = gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(col, "Object Attributes");
  gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);
  renderer = gtk_cell_renderer_text_new ();
  gtk_tree_view_column_pack_start(col, renderer, TRUE);
  gtk_tree_view_column_set_cell_data_func(col, renderer, cell_data_function_col_0, NULL, NULL);
  g_object_set(renderer, "editable", FALSE, NULL);

  col = gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(col, "Attribute Value");
  gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);
  renderer = gtk_cell_renderer_text_new();
  g_signal_connect(renderer, "edited", (GCallback) cell_edited_callback, model);
  gtk_tree_view_column_pack_start(col, renderer, TRUE);
  gtk_tree_view_column_set_cell_data_func(col, renderer, cell_data_function_col_1, NULL, NULL);


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
  GtkWidget *scroll;

  gtk_init (0, 0);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window), "ns-3 Object attributes.");
  gtk_window_set_default_size (GTK_WINDOW (window), 400, 600);
  
  g_signal_connect (window, "delete_event", gtk_main_quit, NULL); /* dirty */

  view = create_view_and_model ();
  scroll = gtk_scrolled_window_new (NULL, NULL);
  gtk_container_add (GTK_CONTAINER (scroll), view);

  GtkWidget *vbox = gtk_vbox_new (FALSE, 5);
  gtk_box_pack_start (GTK_BOX (vbox), scroll, TRUE, TRUE, 0);
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
