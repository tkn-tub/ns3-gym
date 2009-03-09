#include "gtk-config-store.h"
#include "attribute-iterator.h"
#include "raw-text-config.h"
#include "ns3/config.h"
#include "ns3/string.h"
#include "ns3/pointer.h"
#include "ns3/log.h"
#include <gtk/gtk.h>
#include <fstream>


namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("GtkconfigStore");

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
  void Add (ModelNode *node);
  void Remove (void);

  GtkTreeStore *m_treestore;
  std::vector<GtkTreeIter *> m_iters;
};

ModelCreator::ModelCreator ()
{}
void 
ModelCreator::Build (GtkTreeStore *treestore)
{
  m_treestore = treestore;
  m_iters.push_back (0);
  Iterate ();
  NS_ASSERT (m_iters.size () == 1);
}


void
ModelCreator::Add (ModelNode *node)
{
  GtkTreeIter *parent = m_iters.back ();
  GtkTreeIter *current = g_new (GtkTreeIter, 1);
  gtk_tree_store_append (m_treestore, current, parent);
  gtk_tree_store_set (m_treestore, current,
		      COL_NODE, node,
                     -1);
  m_iters.push_back (current);
}
void
ModelCreator::Remove (void)
{
  GtkTreeIter *iter = m_iters.back ();
  g_free (iter);
  m_iters.pop_back ();
}

void 
ModelCreator::DoVisitAttribute (Ptr<Object> object, std::string name)
{
  ModelNode *node = new ModelNode ();
  node->type = ModelNode::NODE_ATTRIBUTE;
  node->object = object;
  node->name = name;
  Add (node);
  Remove ();
}
void 
ModelCreator::DoStartVisitObject (Ptr<Object> object)
{
  ModelNode *node = new ModelNode ();
  node->type = ModelNode::NODE_OBJECT;
  node->object = object;
  Add (node);
}
void 
ModelCreator::DoEndVisitObject (void)
{
  Remove ();
}
void 
ModelCreator::DoStartVisitPointerAttribute (Ptr<Object> object, std::string name, Ptr<Object> value)
{
  ModelNode *node = new ModelNode ();
  node->type = ModelNode::NODE_POINTER;
  node->object = object;
  node->name = name;
  Add (node);
}
void 
ModelCreator::DoEndVisitPointerAttribute (void)
{
  Remove ();
}
void 
ModelCreator::DoStartVisitArrayAttribute (Ptr<Object> object, std::string name, const ObjectVectorValue &vector)
{
  ModelNode *node = new ModelNode ();
  node->type = ModelNode::NODE_VECTOR;
  node->object = object;
  node->name = name;
  Add (node);
}
void 
ModelCreator::DoEndVisitArrayAttribute (void)
{
  Remove ();
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
      g_object_set(renderer, "text", str.Get ().c_str (), (char*)0);
      g_object_set(renderer, "editable", TRUE, (char*)0);
    }
  else
    {
      g_object_set(renderer, "text", "", (char*)0);
      g_object_set(renderer, "editable", FALSE, (char*)0);
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
  g_object_set (renderer, "editable", FALSE, (char*)0);
  switch (node->type) {
  case ModelNode::NODE_OBJECT:
    g_object_set(renderer, "text", node->object->GetInstanceTypeId ().GetName ().c_str (), (char*)0);
    break;
  case ModelNode::NODE_POINTER:
    g_object_set(renderer, "text", node->name.c_str (), (char*)0);
    break;
  case ModelNode::NODE_VECTOR:
    g_object_set(renderer, "text", node->name.c_str (), (char*)0);
    break;
  case ModelNode::NODE_VECTOR_ITEM: {
    std::stringstream oss;
    oss << node->index;
    g_object_set(renderer, "text", oss.str ().c_str (), (char*)0);
  } break;
  case ModelNode::NODE_ATTRIBUTE:
    g_object_set(renderer, "text", node->name.c_str (), (char*)0);
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
  g_return_val_if_fail ( col != 0, -1 );
  g_return_val_if_fail ( col->tree_view != 0, -1 );
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
					  &model, 0, &iter))
    {
      return FALSE;
    }
  if (!gtk_tree_view_get_path_at_pos (GTK_TREE_VIEW (widget),
				      x, y, 0, &column, 0, 0))
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
    uint32_t attrIndex = 0;
    TypeId tid;
    for (tid = node->object->GetInstanceTypeId (); tid.HasParent (); tid = tid.GetParent ())
      {
	for (uint32_t i = 0; i < tid.GetAttributeN (); ++i)
	  {
	    if (tid.GetAttributeName (i) == node->name)
	      {
		attrIndex = i;
		goto out;
	      }
	  }
      }
    out:
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
create_view (GtkTreeStore *model)
{
  GtkTreeViewColumn   *col;
  GtkCellRenderer     *renderer;
  GtkWidget           *view;

  view = gtk_tree_view_new();
  g_object_set (view, "has-tooltip", TRUE, (char*)0);
  g_signal_connect (view, "query-tooltip", (GCallback) cell_tooltip_callback, 0);
  
  gtk_tree_view_set_grid_lines (GTK_TREE_VIEW (view), GTK_TREE_VIEW_GRID_LINES_BOTH);
  gtk_tree_view_set_rules_hint (GTK_TREE_VIEW (view), TRUE);

  col = gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(col, "Object Attributes");
  gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);
  renderer = gtk_cell_renderer_text_new ();
  gtk_tree_view_column_pack_start(col, renderer, TRUE);
  gtk_tree_view_column_set_cell_data_func(col, renderer, cell_data_function_col_0, 0, 0);
  g_object_set(renderer, "editable", FALSE, (char*)0);

  col = gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(col, "Attribute Value");
  gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);
  renderer = gtk_cell_renderer_text_new();
  g_signal_connect(renderer, "edited", (GCallback) cell_edited_callback, model);
  gtk_tree_view_column_pack_start(col, renderer, TRUE);
  gtk_tree_view_column_set_cell_data_func(col, renderer, cell_data_function_col_1, 0, 0);


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
					(char *)0);
  gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER (dialog), TRUE);

  gtk_file_chooser_set_current_name (GTK_FILE_CHOOSER (dialog), "config.txt");


  if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
    {
      char *filename;

      filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
      RawTextConfigSave config;
      config.SetFilename (filename);
      config.Attributes ();
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
					(char *)0);

  if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
    {
      char *filename;

      filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
      RawTextConfigLoad config;
      config.SetFilename (filename);
      config.Attributes ();
    }

  gtk_widget_destroy (dialog);
}

static void 
exit_clicked_callback (GtkButton *button,
		       gpointer   user_data)
{
  gtk_main_quit ();
  gtk_widget_hide (GTK_WIDGET (user_data));
}

static gboolean
delete_event_callback (GtkWidget *widget,
		       GdkEvent  *event,
		       gpointer   user_data)
{
  gtk_main_quit ();
  gtk_widget_hide (GTK_WIDGET (user_data));
  return TRUE;
}

static gboolean 
clean_model_callback (GtkTreeModel *model,
		      GtkTreePath *path,
		      GtkTreeIter *iter,
		      gpointer data)
{
  ModelNode *node;
  gtk_tree_model_get (GTK_TREE_MODEL (model), iter, 
		      COL_NODE, &node, 
		      -1);
  delete node;
  gtk_tree_store_set (GTK_TREE_STORE (model), iter,
		      COL_NODE, (ModelNode*)0,
		      -1);
  return FALSE;
}

GtkConfigStore::GtkConfigStore ()
{}

void 
GtkConfigStore::ConfigureDefaults (void)
{}

void 
GtkConfigStore::ConfigureAttributes (void)
{
  GtkWidget *window;
  GtkWidget *view;
  GtkWidget *scroll;

  gtk_init (0, 0);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window), "ns-3 Object attributes.");
  gtk_window_set_default_size (GTK_WINDOW (window), 600, 600);
  
  g_signal_connect (window, "delete_event", (GCallback)delete_event_callback, window);


  GtkTreeStore *model = gtk_tree_store_new (COL_LAST, G_TYPE_POINTER);
  ModelCreator creator;
  creator.Build (model);

  view = create_view (model);
  scroll = gtk_scrolled_window_new (0, 0);
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
  GtkWidget *exit = gtk_button_new_with_label ("Run Simulation");
  g_signal_connect (exit, "clicked",  (GCallback) exit_clicked_callback, window);
  gtk_box_pack_end (GTK_BOX (hbox), exit, FALSE, FALSE, 0);

  gtk_container_add (GTK_CONTAINER (window), vbox);

  gtk_widget_show_all (window);

  gtk_main ();

  gtk_tree_model_foreach (GTK_TREE_MODEL (model), 
			  clean_model_callback, 
			  0);

  gtk_widget_destroy (window);
}

} // namespace ns3
