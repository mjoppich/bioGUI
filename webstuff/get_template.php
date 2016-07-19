<?php

require("php_db_functions.php");


$conn = connectDB();

$templ_id = $_GET['templid'];

$query_templates = "select template from templates where id = $templ_id;";

$result = queryWithError($conn, $query_templates);

if ($result === FALSE)
{
    die ("Error retrieving templates");
}

while($row = $result->fetch_assoc())
{

    $db_template = $row['template'];
    
    print($db_template);
    
}


?>