<?
   $edit_type = ($object_id == 0 ? "add" : "edit");
   $what = ($type == "person" ? "a person" : "an user");
   print "<h1>" . ucfirst($edit_type) . " $what</h1>";
   
   $parts[] = array("form_required","Required");
   $parts[] = array("form_contact","Contact");
   if ($type == "user") $parts[] = array("form_user","User");
         
   $form = new Form("","get",$parts);
   $form->start("form_required");
   print "<tr><td>First name</td><td><input type='text' name='name' /></td></tr>";
   print "<tr><td>Middle name</td><td><input type='text' name='name' /></td></tr>";
   print "<tr><td>Last name</td><td><input type='text' name='name' /></td></tr>";
   $form->start("form_contact");
   print "<tr><td>e-mail</td><td><input type='text' name='name' /></td></tr>";
   print "<tr><td>Home page</td><td><input type='text' name='name' /></td></tr>";   
   if ($type == "user") {
      $form->start("form_user");
      print "<tr><td>login</td><td><input type='text' name='name' /></td></tr>";
      print "<tr><td>Password</td><td><input type='password' name='name' /></td></tr>";   
      print "<tr><td>Password (confirm)</td><td><input type='password' name='name' /></td></tr>";   
   }
   $form->end();
?>