<?

session_start();

require_once("DB.php");
require_once("settings.php");

$unibib_pages = array(
   "main" => array("Main page","Home","index",true),
   "users" => array("People, users and groups","People","users",true),
   "folders" => array("Organisation: folders","Folders","folders",true),
   "publications" => array("Publications","Publications","publications",true),
   // "Hidden" pages
   "login" => array("Login and logout","Login","loginout",false)
);


define(UNIBIB_OK,"info");
define(UNIBIB_WARN,"warning");
define(UNIBIB_ERROR,"error");

class Unibib {
   var $page_id;
   var $db;
   var $header_sent;
   var $messages;
   
   /** Constructor
   */
   function Unibib($pid) {
      global $unibib_preferences;
      $this->header_sent = false;
      $this->db =& DB::connect($unibib_preferences["dsn"], $options);
      $this->db->autoCommit(false);
      if (DB::isError($db)) {
         die($this->db->getMessage());
      }
      for($i = 0; $i < sizeof($_GET["message"]); $i++)
         $this->add_message($_GET["message"][$i],$_GET["code"][$i]);
      $this->page_id = $pid;
   }
   
   function add_message($message,$type) {
      $this->messages[] = array($message,$type);
   }
   
   /** Checks if the user can do the given action
   *
   * @param write boolean which is true if the user request write access
   * @param what is the name of the resource
   * @param options is an array with specific resource options
   */
   function is_granted($write, $what, $options = false) {
      if (!$write) return true;
      switch($what) {
         case "user": return $_SESSION["grants"]["users"];
         case "group": return $_SESSION["grants"]["groups"];
         default:
            return $_SESSION["login"] != "";
      }
      
   }
   
   /** Outputs an error message and exit */
   function error($message) {
      $this->header();
      ?><div class="error"><?=$message?></div><?
      $this->footer();
      exit();
   }
   
   /** Outputs the header 
   */
   function header() {
      global $unibib_pages;
      if ($this->header_sent) return;
      $this->header_sent = true;
      
      //print '< ?xml version="1.0" encoding="UTF-8"? >' . "\n"; 
      ?>
            <!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
                  <html xmlns="http://www.w3.org/1999/xhtml">

                  <head>
                  <title>UniBib - <?=$unibib_pages[$this->page_id][1]?></title>
                  <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
                  <link rel="stylesheet" type="text/css" href="unibib.css" />
            <script src="unibib.js" type="text/javascript"></script>
            </head>
                  <body>
            <div id="title">
            UniBib - <?=$unibib_pages[$this->page_id][0]?>
            </div>
            
            

       <?
            print "<div class=\"status\">";
      if ($_SESSION["login"]) print "Logged in as <b>$_SESSION[login]</b> (<a href=\"loginout.php?action=logout\">logout</a>)";
      else print "<a href=\"loginout.php\">Log in</a>";
      if (sizeof($this->messages) > 0) 
         foreach($this->messages as $message) {
         print "<div class='$message[1]'>$message[0]</div>";
      }
      print "</div>\n";

            $this->print_tabs($this->page_id, $unibib_pages);
      print "<div id='main_div' class=\"box\">";
   }
   
   function footer() {
      if (!$this->header_sent) $this->header();

      ?></div></body></html><?
   }
   
   function print_tabs($sid, $list) {
      print '<div class="tab">';
      foreach($list as $id => $data) {
         if ($data[3]) print "<a href=\"$data[2]\"" . ($id == $sid ? " class=\"selected\"" : "") . ">$data[1]</a>";
      }
      print "</div>\n";
   }
}

/* Class forms */
class Form {
   var $states;
   function Form($action,$method="GET") {
      print "<form action=\"$action\" method=\"$method\">\n";
      $this->hidden("timestamp",mktime());
   }
   
   function start_fields(&$tabs) {
      print '<div class="tab">';
      for($i = 0; $i < sizeof($tabs); $i++) {
         print "<a href=\"javascript:void(0)\" onclick=\"javascript:select_form_tab(this,'" . $tabs[$i][0] . "','" . $tabs[0][0] . "')\"" . ($i == 0 ? " class=\"selected\"" : "") . ">" . $tabs[$i][1] . " </a>";
      }
      print "</div>\n";
      print "<div class=\"box\">\n";
   }
   
   function end_fields() {
      print "</div>\n";
   }
   
   function start_list($id, $visible = true) {
      if ($visible) {
         print "<table id=\"$id\" style=\"position: relative; visibility: visible\"><tbody>";
      } else {
         print "<table  id=\"$id\" style=\"position: absolute; visibility: hidden\"><tbody>";
      }
   }
   function end_list() { print "</tbody></table>\n"; }
   
   function end($name = "save") {
      print "<div><input type=\"submit\" value=\"$name\" name=\"action\" /></div>";
      print "</form>\n";
   }
   
   /** A select field
   */
   function select($title, $tip, $list, $varname, $old, $info, $null_ok = false) {
      print "<tr><td><span title='$tip'>$title</span></td><td><select name=\"$varname\">\n";
      if ($null_ok) print "<option value=\"$null_ok\">-</option>\n";
      foreach($list as $key => $value) {
         print "<option value=\"$key\"" . ($old == $key ? " selected=\"selected\"" : "") . ">$value</option>\n";
      }
      print "</select></td></tr>\n";
   }
   
   /** A text field
   */
   function text($title, $tip, $varname, $old, $info) {
      print "<tr><td><span title='$tip'>$title</span></td><td><input type=\"text\" name=\"$varname\" value=\"$old\"/></td></tr>\n";
   }
   /** A text field
   */
   function password($title, $tip, $varname, $old, $info) {
      print "<tr><td><span title='$tip'>$title</span></td><td><input type=\"password\" name=\"$varname\" value=\"$old\"/></td></tr>\n";
   }
   function hidden($name,$value) {
      print "<input type=\"hidden\" name=\"$name\" value=\"" . htmlspecialchars($value) . "\"/>";
   } 
   function field(&$data, $part, $name) {
      global $UB_FIELDS;
      $field = &$UB_FIELDS[$part][$name];
      print "<tr><td" . ($field[4] ? " title='$field[4]'" : "") . ">{$field[3]}</td><td>";
      $max = ($field[1] > 50 ? 50 : $field[1]); 
      $old = htmlspecialchars($data->get($part,$name));
      switch($field[0]) {
         case "text": case "password":
            print "<input type=\"$field[0]\" name=\"{$part}[{$name}]\" size=\"$max\" maxlength=\"$field[1]\" value=\"$old\"/>";
      }
      print "</td></tr>\n";
   }
}

?>