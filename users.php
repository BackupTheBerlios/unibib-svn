<?
require_once("include/unibib.inc");
$unibib = new Unibib("users");
$unibib->header();

switch($_GET["action"]) {
   case "add_person": $type="person"; require_once("scripts/edit_people.inc"); break;
   case "add_user": $type="user"; require_once("scripts/edit_people.inc"); break;
   default:
?>

In this area you can add persons and users. Users are persons who can manage have a write access to this database. You can now:
<ul>
<li><a href="?action=add_person">Add a person</a></li>
<li><a href="?action=add_user">Add an user</a></li>
<li>See the list of people</li>
</ul>

<? 
}
$unibib->footer(); 
?>