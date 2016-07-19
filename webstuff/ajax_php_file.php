<?php

require(php_db_functions.php);

$username = $_POST["username"];
$usermail = $_POST["usermail"];
$template = $_POST["template"];
$template_name = $_POST["template_name"];
$template_type = $_POST['template_type'];

print_r($_POST);

$conn = connectDB();

$find_user = "select * from users where name like '".$username."' and email = '". $usermail ."';";
$add_user = "insert into users (email, name) values ('$usermail', '$username')";

$result = queryWithError($conn, $find_user);
$user_db_id = getIDOrCreate($conn, $find_user, $add_user);

$add_template = "insert into templates (displayname, template, type, user) values ('$template_name','$template',$template_type ,$user_db_id);";
$find_template = "select * from templates where user = $user_db_id and type = $template_type and displayname = '$template_name';";

$template_db_id = getIDOrCreate($conn, $find_template, $add_template);

print($find_user);
print($add_user);
print($find_template);
print($add_template);

$conn->close();

exit("0");


if(isset($_FILES["file"]["type"]))
{
$validextensions = array("jpeg", "jpg", "png");
$temporary = explode(".", $_FILES["file"]["name"]);
$file_extension = end($temporary);
if ((($_FILES["file"]["type"] == "image/png") || ($_FILES["file"]["type"] == "image/jpg") || ($_FILES["file"]["type"] == "image/jpeg")
) && ($_FILES["file"]["size"] < 100000)//Approx. 100kb files can be uploaded.
&& in_array($file_extension, $validextensions)) {
if ($_FILES["file"]["error"] > 0)
{
echo "Return Code: " . $_FILES["file"]["error"] . "<br/><br/>";
}
else
{
if (file_exists("upload/" . $_FILES["file"]["name"])) {
echo $_FILES["file"]["name"] . " <span id='invalid'><b>already exists.</b></span> ";
}
else
{
$sourcePath = $_FILES['file']['tmp_name']; // Storing source path of the file in a variable
$targetPath = "./upload/".$_FILES['file']['name']; // Target path where file is to be stored
move_uploaded_file($sourcePath,$targetPath) ; // Moving Uploaded file
echo "<span id='success'>Image Uploaded Successfully...!!</span><br/>";
echo "<br/><b>File Name:</b> " . $_FILES["file"]["name"] . "<br>";
echo "<b>Type:</b> " . $_FILES["file"]["type"] . "<br>";
echo "<b>Size:</b> " . ($_FILES["file"]["size"] / 1024) . " kB<br>";
echo "<b>Temp file:</b> " . $_FILES["file"]["tmp_name"] . "<br>";
}
}
}
else
{
echo "<span id='invalid'>***Invalid file Size or Type***<span>";
}
}
?>