<?php

function queryWithError($conn, $sql_query)
{
    $result = $conn->query($sql_query);

    if ( $result === FALSE)
    {
        print( "error: " . $sql_query . "  " . $conn->error );

        return FALSE;
    }

    return $result;
}

function getIDOrCreate($conn, $sql_get, $sql_insert)
{

    $result = queryWithError($conn, $sql_get);

    if (($result === FALSE) || ($result->num_rows == 0))
    {
        $result = queryWithError($conn, $sql_insert);

        if ($result === FALSE)
        {
            die (" error inserting new id: " + $conn->error);
        }

        return mysqli_insert_id($conn);
    } else {

        $db_id = -1;
        while($row = $result->fetch_assoc())
            $db_id = $row['id'];

        return $db_id;
    }

}

function connectDB()
{
    $db_servername = "localhost";
    $db_username = "root";
    $db_password = "";

    // Create connection
    $conn = new mysqli($db_servername, $db_username, $db_password, "gui");

    if ($conn->connect_error) {
        die("Connection failed: " . $conn->connect_error);
    }

    return $conn;
}

?>