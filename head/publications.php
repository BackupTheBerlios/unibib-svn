<?
require_once("include/unibib.inc");
$unibib = new Unibib("publications");
$unibib->header();

if ($_GET["action"] == "add") {
   require("scripts/edit_publication.inc");
} else {

?>

<dl>
<dt>Add a publication</dt>
<dd>You can add a <a href="<?=$PHP_SELF?>?action=add&amp;type=article">journal article</a>, ...</dd>

<dt>Import publication list</dt>
<dd>Import <a href="">bibtex</a>, <a href="">bibtexML</a>, ... </dd>

<dt>Search</dt>
<dd>DIALOG</dd>
</dl>

<? 
}
$unibib->footer(); 
?>