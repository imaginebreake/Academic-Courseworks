<?php
    session_start();
    // check login
    if(!isset($_SESSION['userName']))
    {
        header('Location: ./../signin_up.php');
    }
    // get room
    else
    {
        try {
            // set up database
            $conn = new PDO("mysql:host=localhost;dbname=test",
                            "username", "passwd");
            $conn->setAttribute(PDO::ATTR_ERRMODE,
                                PDO::ERRMODE_EXCEPTION);
            
            $startDate = $_GET["startdate"];
            $endDate = $_GET["enddate"];
            $roomType = $_GET["roomtype"];
            
            // query rooms
            $sql = "SELECT * FROM room WHERE rID <> ALL(SELECT rID FROM book WHERE rID = ANY(SELECT rID FROM room WHERE (room_type = '". $roomType ."')) AND (status <> -1) AND (((startDate < '". $endDate ."') AND (endDate >= '". $endDate ."')) OR ((startDate <= '". $startDate ."') AND (endDate > '". $startDate ."'))) ) AND (room_type = '". $roomType ."')";

            // echo $sql . "<br>";

            // return rooms in json
            $rows = $conn->query($sql);
            $result = array();
            if ($rows->rowCount() != 0) {
                foreach ($rows as $row) {
                    array_push($result, $row);
                }
            }
            $jsdata=json_encode($result);
            echo $jsdata;
            $conn = NULL;
        } catch (PDOException $e) {
            // 503 if database error
            header('HTTP/1.1 503 Service Temporarily Unavailable');
            header('Status: 503 Service Temporarily Unavailable');
            header('Location: ./../503.php');      
        }
    }
?>
