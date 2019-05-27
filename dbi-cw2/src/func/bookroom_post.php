<?php
session_start();
try {

    // set up database
    $conn = new PDO(
        "mysql:host=localhost;dbname=test",
        "scytg1",
        "123"
    );
    $conn->setAttribute(
        PDO::ATTR_ERRMODE,
        PDO::ERRMODE_EXCEPTION
    );

    // get post info
    $startDate = $_POST["startdate"];
    $endDate = $_POST["enddate"];
    $roomNumber = $_POST["roomID"];
    $userName = $_POST["username"];

    // get existing cID
    $sql = "SELECT cID FROM customer WHERE username = '" . $userName . "'";
    $rows = $conn->query($sql);
    $customterID = $rows->fetch()['cID'];
    // print_r($customterID);

    // get existing rID
    $sql = "SELECT rID FROM room WHERE room_number = '" . $roomNumber . "'";
    $rows = $conn->query($sql);
    $roomID = $rows->fetch()['rID'];
    // print_r($roomID);

    // book room
    $data = array($startDate, $endDate, 0, $customterID, $roomID);
    $conn->prepare("INSERT INTO book VALUES (null,?,?,?,?,?)")->execute($data);

    // return
    $conn = NULL;
    $data = array("startDate" => $startDate, "endDate" => $endDate, "userName" => $userName, "roomNumber" => $roomNumber);
    $jsdata = json_encode($data);
    echo $jsdata;
} catch (PDOException $e) {
    // 503 if database error
    header('HTTP/1.1 503 Service Temporarily Unavailable');
    header('Status: 503 Service Temporarily Unavailable');
    header('Location: ./../503.php');
}

?>