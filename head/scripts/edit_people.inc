<?
   
   require_once("include/objects.inc");
   switch($_REQUEST["action"]) {
    case "save":
      $type=$_REQUEST["type"];
      if ($type == "user") $data = new UBUser();
      else $data = new UBPerson();
      $data->load_from_request($_REQUEST);
      $data->id = $_REQUEST["id"];
      if ($_REQUEST["timestamp"] < $_SESSION["form_timestamp"])
         $unibib->add_message("This form has already been submitted.",UNIBIB_WARN);
      else {
         $_SESSION["form_timestamp"] = time();
      $err = $data->update();
      if (DB::isError($err)) {
         $unibib->add_message("Error while asking the updating database: : " . $err->getUserInfo(),UNIBIB_ERROR);
         $unibib->db->rollback();
        
      } else {
         $unibib->add_message("Updated database (object id {$data->id})",UNIBIB_OK);
         $unibib->db->commit();
         header("Location: $PHP_SELF?message[]=" . rawurlencode("Updated database (object id {$data->id})") . "&code[]=" . UNIBIB_OK);
         break; // exit
      }
      }
   default:
      $unibib->header();
      if ($_REQUEST["action"] != "save") {     
      if ($type == "person") $data = new UBPerson();
      else $data = new UBUser();
      } 
     
   $edit_type = ($object_id == 0 ? "add" : "edit");
   $what = ($type == "person" ? "a person" : "an user");
   print "<h1>" . ucfirst($edit_type) . " $what</h1>";
   
   $parts[] = array("form_required","Required");
   $parts[] = array("form_contact","Contact");
   if ($type == "user") {
      $parts[] = array("form_user","User"); 
      $parts[] = array("form_group","Group");
   }
            
   $form = new Form("","get");
   $form->start_fields($parts);
   $form->start_list("form_required",true);
   $form->hidden("id",$data->id);
   $form->hidden("type",$type);
   $form->field($data,"persons","firstname");
   $form->field($data,"persons","middlename");
   $form->field($data,"persons","lastname");
   $form->end_list();
   
   $form->start_list("form_contact",false);
   $form->field($data,"persons","email");
   $form->field($data,"persons","home_page");
   $form->end_list();
      
   if ($type == "user") {
      $form->start_list("form_user",false);
      $form->field($data,"users","login");
      $form->field($data,"users","password");
      $form->end_list();
      $form->start_list("form_group",false);
      $unibib->db->query("SELECT * FROM unibib.groups");
      $form->end_list();
   }
   $form->end_fields();
   $form->end();
   }
?>