<?php
    session_start();

    // check login status
    $username = "";
    $loginFlag = true;

    if (!isset($_SESSION['userName'])) // If session is not set then redirect to Login Page
    {
        $loginFlag = false;
    }

    // return 404 if not login
    if ($loginFlag == false)
    {
        header('HTTP/1.1 404 Not Found'); 
        header("status: 404 Not Found"); 
        exit();
    }
    else {
        try {
            // set up datebase
	        $conn = new PDO("mysql:host=localhost;dbname=test",
						"username", "passwd");
            $conn->setAttribute(PDO::ATTR_ERRMODE,
                                PDO::ERRMODE_EXCEPTION);
            
            $sql = null;
            if ($_SESSION['userGroup'] == 'customer')
            {
                // get valid books of current customer
                $sql = "SELECT bID,startDate,endDate,status,username,room_number,room_type FROM (book INNER JOIN customer ON book.cID = customer.cID) INNER JOIN room ON book.rID = room.rID WHERE status <> -1 and username = '" . $_SESSION['userName'] . "'";
            }
            else {
                // get all valid books
                $sql = "SELECT bID,startDate,endDate,status,username,room_number,room_type FROM (book INNER JOIN customer ON book.cID = customer.cID) INNER JOIN room ON book.rID = room.rID WHERE status <> -1";
            }
        
            // return books in json
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