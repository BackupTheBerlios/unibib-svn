<?

/** A hierarchical structure with a database backend
*/


class DBTree {
   var $db;
   var $table;
   var $this->left;
   var $this->right;
   var $this->id;
   
   /** Constructor
   *
   * Construct the tree  
   */ 
   function DBTree(&$db, $table) {
      $this->db = &$db;
      $this->table = $table;
      // options (default values)
      $this->left = "l";
      $this->right = "r";
      $this->id = "id";
   }
   
   /** Add a child (first child)
   */
   function add_child($parent_id, &$values) {
      // Get the data from the parent id
      $parent = $db->getRow("SELECT $this->left, $this->right FROM $this->table WHERE $this->id=$parent_id",array(),DB_FETCHMODE_ARRAY);
      if (DB::isError($err)) return $err;
      return __add_node($parent[0],$values);
   }
   
   /** Add a node after another
   */
   function add_child($sibling_id, &$values) {
      // Get the data from the parent id
      $sibling = $db->getRow("SELECT $this->left, $this->right FROM $this->table WHERE $this->id=$sibling_id",array(),DB_FETCHMODE_ARRAY);      
      return __add_node($sibling[1], $values);
   }
   
   function get_children($id) {
      $id = $this->db->getAll("SELECT * FROM $this->table WHERE $this->parent=?",array($id));
   }
   
   function get_root() {
      return $this->db->getRow("SELECT * FROM $this->table WHERE $this->parent=0");
   }

   // Private methods
   
   /** Add a node
   */
   function __add_node($after, &$values) {
      // Prepare the insertion
      $err = $db->query("UPDATE $this->table SET $this->left = $this->left  + 1  WHERE $this->left > $after");
      if (DB::isError($err)) return $err;
      $err = $db->query("UPDATE $this->table SET $this->right = $this->right + 1  WHERE $this->right >= $after");
      if (DB::isError($err)) return $err;
      
      // Get the new id 
      $our_values["id"] = nextId($this->table . "_seq",true);
      if (DB::isError($our_values["id"]) return $our_values["id"];
      
      // Insert the element
      $our_values[$this->left] = $our_values[$this->right] = $after + 1;
      return $db->autoExecute($this->table,array_merge($our_values,$values));    
   }

} // end of class Tree

?>