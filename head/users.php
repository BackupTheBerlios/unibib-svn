<?
require_once("include/unibib.inc");
$unibib = new Unibib("users");

switch($_REQUEST["action"]) {
   case "save":  require_once("scripts/edit_people.inc"); break;
   case "add_person": $unibib->header(); $type="person"; require_once("scripts/edit_people.inc"); break;
   case "add_user": $unibib->header(); $type="user"; require_once("scripts/edit_people.inc"); break;
   default:
switch($_REQUEST["view"]) {
   case "list":
      $qh = false; 
      if (preg_match('/^\w+$/',$_REQUEST["alpha"])) 
         $qh = $unibib->db->query("SELECT * FROM unibib.persons WHERE lastname like '$_REQUEST[alpha]%' ORDER by lastname,firstname");
      if (DB::isError($qh)) $unibib->error($qh->getUserInfo());
      $unibib->header();
      print "<h1>Alphabetical list</h1>"; 
      
      $alpha = array();
      for($i = ord('A'); $i <= ord('Z') ; $i++) $alpha[chr($i)] = array(chr($i),chr($i),"$PHP_SELF?view=list&amp;alpha=" . chr($i),true);
      $unibib->print_tabs($_REQUEST["alpha"],$alpha); 
      print "<div class='box'>";
      if ($qh) {
      $k = 0; 
      while ($row = $qh->fetchRow()) { 
         print "<div class='list_$k'>$row[3], $row[1]</div>"; 
         $k = ($k + 1) % 2; 
      } 
} else print "<div class='info'>Please choose a letter</div>\n";   
      print "</div>";  
      break; 
   default:
      $unibib->header();
?>
In this area you can add persons and users. Users are persons who can manage have a write access to this database. You can now:
<ul>
<? if ($unibib->is_granted(true,"person")) {?><li><a href="?action=add_person">Add a person</a></li><?}?>
<? if ($unibib->is_granted(true,"user")) {?><li><a href="?action=add_user">Add an user</a></li><?}?>
<li><a href="<?=$PHP_SELF?>?view=list">See the list of people</a></li>
</ul>

<? 
}
}
$unibib->footer(); 
?>