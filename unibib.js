// Unibib javascript resources


// Select a form tab
// x is the element on which the user clicked
// id is the div id of the form area content
// first_id is the div id of the first form aree
function select_form_tab(x,id,first_id) {
  var y = document.getElementById(id);
  if (!y) { alert("Bug: no element for a form id '" + id + "'"); return false; }
  
  // First, deselect all others
  var children = x.parentNode.childNodes;
  for(var i = 0; i < children.length; i++) {
     children[i].setAttribute("class","");
  }
  x.setAttribute("class","selected");
  var old = x.parentNode.old;
  if (!old) old = document.getElementById(first_id);
  if (old) {
     old.style.visibility = "hidden";
     old.style.position = "absolute";
  }
  y.style.visibility = "visible";
  y.style.position = "relative";
  x.parentNode.old = y;
  return false;
}