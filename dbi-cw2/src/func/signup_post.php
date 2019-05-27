<?php
session_start();
// check post info
if ( ( $_POST['username'] != null ) && ( $_POST['password'] != null ) && ( $_POST['truename'] != null) && ( $_POST['telephone'] != null) && (( $_POST['staffid'] != null) || ( $_POST['passport'] != null)) ) {

    // set post info
    $userName = $_POST['username'];
    $passWord = $_POST['password'];
    $userGoup = $_POST['inlineRadioOptions'];
    $truename = $_POST['truename'];
    $staffid = $_POST['staffid'];
    $passport = $_POST['passport'];
    $telephone = $_POST['telephone'];
    $email = $_POST['email'];

    try {
        // set up database
        $conn = new PDO("mysql:host=localhost;dbname=test",
        "username", "passwd");
        $conn->setAttribute(PDO::ATTR_ERRMODE,
        PDO::ERRMODE_EXCEPTION);

        // find customer with same username
        $sql = "SELECT * FROM customer WHERE username ='" . $userName . "'";
        $rows_customer = $conn->query($sql);
        $num_c = $rows_customer->rowCount();

        // find staff with same username
        $sql = "SELECT * FROM staff WHERE username ='" . $userName . "'";
        $rows_staff = $conn->query($sql);
        $num_s = $rows_staff->rowCount();

        // return to false page if username duplicate
        if (($num_c != 0) || ($num_s != 0)) {
        header('Location: ./../signin_up.php?signup=false');
        }
        else {
            // add customer user
            if ( $userGoup == "customer" ) {
            $data = array($userName, $passWord, $truename, $passport, $telephone, $email);
            $conn->prepare("INSERT INTO customer VALUES (null,?,?,?,?,?,?)")->execute($data);
            }
            else {
            // add staff user
            $data = array($userName, $passWord, $truename, $staffid, $telephone, $email);
            $conn->prepare("INSERT INTO staff VALUES (null,?,?,?,?,?,?)")->execute($data);
            } 

            // set session and return to index page
            $_SESSION['userName'] = $userName;
            $_SESSION['userGroup'] = $userGoup;
            header('Location: ./../index.php');
        }

        $conn = NULL;

    } catch (PDOException $e) {
        // 503 if database error
        header('HTTP/1.1 503 Service Temporarily Unavailable');
        header('Status: 503 Service Temporarily Unavailable');
        header('Location: ./../503.php');
    }

}
else {
    header('Location: ./../signin_up.php?signup=false');
}

?>
