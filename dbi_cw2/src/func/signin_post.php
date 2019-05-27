<?php
session_start();

// check post params
if ( ( $_POST['username'] != null ) && ( $_POST['password'] != null ) ) {
    $userName = $_POST['username'];
    $passWord = $_POST['password'];
    
    try {
        // set up database
        $conn = new PDO("mysql:host=localhost;dbname=test",
                                            "username", "passwd");

        $conn->setAttribute(PDO::ATTR_ERRMODE,
                            PDO::ERRMODE_EXCEPTION);

        $signinFlag = false;

        // staff login
        if ( isset($_POST['isStaff']) == True ) {
            // check username and password in database
            $sql = "SELECT * FROM staff WHERE username = '$userName' AND password = '$passWord';";
            $rows = $conn->query($sql);
            $conn = NULL;

            if ($rows->rowCount() != 0) {
                $_SESSION['userName'] = $userName;
                $_SESSION['userGroup'] = "staff";
                $signinFlag = true;
                header('Location: ./../index.php');
            }
            else {
                header('Location: ./../signin_up.php?signin=false');
            }
        }
        // customer login
        else {
            // check username and password in database
            $sql = "SELECT * FROM customer WHERE username = '$userName' AND password = '$passWord';";
            $rows = $conn->query($sql);
            $conn = NULL;
        
            if ($rows->rowCount() != 0) {
                $_SESSION['userName'] = $userName;
                $_SESSION['userGroup'] = "customer";
                $signinFlag = true;
            }
            
            if ($signinFlag == true) {
                header('Location: ./../index.php');
            }
            else {
                // return to signin page and let is display false
                header('Location: ./../signin_up.php?signin=false');
            }
    
        }
    } catch (PDOException $e) {
        // return 503 if database error
        header('HTTP/1.1 503 Service Temporarily Unavailable');
        header('Status: 503 Service Temporarily Unavailable');
        header('Location: ./../503.php');        
    }
}
else {
    header('Location: ./../signin_up.php?signin=false');
}

?>
