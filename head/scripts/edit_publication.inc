<?

  $type = $_GET["type"];
  $info = $unibib->db->tableInfo('publications.article');
//  print_r($info);

 $parts[] = array("main","Main");
 $parts[] = array("authors","Authors");
 if ($type == "article") $parts[] = array("journal","Journal");
 $parts[] = array("keywords","Keywords");

 $form = new Form("$PHP_SELF","get");
 
 $form->start_fields($parts);
   
 // Common part
   $form->start_list("main");
   $form->text("Title","Full title of the publication","publication[title]",$publication,$info);
   for($i = 1; $i < 13; $i++) $months[] = date("F", mktime(0,0,0,$i,0,0));
   $form->select("Month","Month this publication was published (optional)",$months,"publication[year]",$publication["year"],$month,"-");
   $form->text("Year","Year the publication was published","publication[year]",$publication,$info);
   $form->text("URL","URL which provides an access to this publication","publication[url]",$publication,$info);
   $form->text("Language","URL which provides an access to this publication","publication[url]",$publication,$info);
   $form->end_list(); 
   
   $form->start_list("authors",false);
   $form->end_list();
   
   if ($type == "article") {
      $form->start_list("journal");
      $form->text("From page n°","First page in the journal","article[first_page]",$article[first_page],$info);
      $form->text("to page n°","Last page in the journal","article[last_page]",$article[last_page],$info);
      $form->end_list();
   }
   
   $form->end_fields();
  $form->end(); 
?>