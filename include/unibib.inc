<?

$unibib_pages = array(
   "main" => array("Main page","Home","index"),
   "users" => array("People and users","People","users"),
   "publications" => array("Publications","Publications","publications") 
);

class Unibib {
   var $page_id;
   function unibib($pid) {
      $this->page_id = $pid;
   }
   
   function header() {
      global $unibib_pages;
      //print '< ?xml version="1.0" encoding="UTF-8"? >' . "\n"; 
      ?>
            <!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
                  <html xmlns="http://www.w3.org/1999/xhtml">

                  <head>
                  <title>UniBib - <?=$unibib_pages[$this->page_id][1]?></title>
                  <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
                  <link rel="stylesheet" type="text/css" href="unibib.css" />
            <script src="unibib.js" type="text/javascript" />
            </head>
                  <body>
            <div id="title">
            UniBib - <?=$unibib_pages[$this->page_id][0]?>
            </div>

       <?
            $this->print_tabs($this->page_id, $unibib_pages);
      print "<div id='main' class=\"box\">";
   }
   
   function footer() {
      ?></div></body></html><?
   }
   
   function print_tabs($sid, $list) {
      print '<div class="tab">';
      foreach($list as $id => $data) {
         print "<a href=\"$data[2].php\"" . ($id == $sid ? " class=\"selected\"" : "") . ">$data[1]</a>";
      }
      print "</div>\n";
   }
}

/* Class forms */
class Form {
   var $state;
   function Form($action,$method="GET", $tabs) {
      print "<form action=\"$action\" method=\"$method\">\n";
      print '<div class="tab">';
      for($i = 0; $i < sizeof($tabs); $i++) {
         print "<a href=\"javascript:void(0)\" onclick=\"javascript:select_form_tab(this,'" . $tabs[$i][0] . "','" . $tabs[0][0] . "')\"" . ($i == 0 ? " class=\"selected\"" : "") . ">" . $tabs[$i][1] . " </a>";
      }
      print "</div>\n";
      print "<div class=\"box\">\n";
      $this->state = 0;
   }
   function start($id) {
      if ($this->state == 0) {
         print "<table id=\"$id\" style=\"position: relative; visibility: visible\"><tbody>";
         $this->state = 1;
      } else {
         if ($this->state == 1) print "</tbody></table>\n";
         print "<table  id=\"$id\" style=\"position: absolute; visibility: hidden\"><tbody>";
      }
   }
   function end() {
      if ($this->state == 1) print "</tbody></table>\n";
      print "<input type=\"submit\" value=\"save\" name=\"action\" />";
      print "</div>";
      print "</form>\n";
   }
}

?>