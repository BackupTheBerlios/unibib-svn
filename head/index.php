<?
require_once("include/unibib.inc");
$unibib = new unibib("main");
$unibib->header();

if ($_SESSION["grants"]["users"]) print "<div>You can create and edit <em>users</em>.</div>";
if ($_SESSION["grants"]["groups"]) print "<div>You can create and edit <em>groups</em>.</div>";


$unibib->footer(); ?>