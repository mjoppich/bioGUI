<?php

require("php_db_functions.php");


$conn = connectDB();

$query_templates = "select displayname, templates.id as id, timestamp, type, email, name from templates join users on templates.user = users.id ";

$result = queryWithError($conn, $query_templates);

if ($result === FALSE)
{
    die ("Error retrieving templates");
}

while($row = $result->fetch_assoc())
{

    $db_id = $row['id'];
    $db_type = $row['type'];
    $db_template_name = $row['displayname'];
    $db_timestamp = $row['timestamp'];
    $db_username = $row['name'];

    print( $db_id . "\t" . $db_type . "\t" . $db_template_name . "\t" . $db_timestamp . "\t" . $db_username);
}


?>