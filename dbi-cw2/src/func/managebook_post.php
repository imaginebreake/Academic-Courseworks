<?php
    session_start();

    // check login
    $username = "";
    $loginFlag = true;

    if (!isset($_SESSION['userName'])) // If session is not set then redirect to Login Page
    {
        $loginFlag = false;
    }

    // return 404 if not login account
    if ($loginFlag == false)
    {
        header('HTTP/1.1 404 Not Found'); 
        header("status: 404 Not Found"); 
        exit();
    }
    else {
        try {
            // set up database
	        $conn = new PDO("mysql:host=localhost;dbname=test",
						"username", "passwd");
            $conn->setAttribute(PDO::ATTR_ERRMODE,
                                PDO::ERRMODE_EXCEPTION);
            
            // update book in databse
            if ( ( $_POST['bID'] != null ) && ( $_POST['opt'] != null ) ) {
                $bID = $_POST['bID'];
                $opt = $_POST['opt'];
                $checkValid = true;

                if ($_SESSION['userGroup'] == 'customer') {
                        $checkValid = false;
                        // get cID,username
                        $sql = "SELECT book.bID,book.cID,customer.username FROM book INNER JOIN customer ON customer.cID=book.cID WHERE bID = '" . $bID . "'";
                        $rows = $conn->query($sql);
                        $username = $rows->fetch()['username'];
                        if ($username != null) {
                            if ($username == $_SESSION['userName']) {
                                $checkValid = true;
                            }
                        }
                }

                if ($opt == "2") {
                    $opt = "-1";
                }

                if ($checkValid) {
                    $sql = "UPDATE book SET status= ? WHERE bID = ?";
                    $conn->prepare($sql)->execute([$opt, $bID]);
                    // return success
                    $jsdata=json_encode(array("success"));
                    echo $jsdata;
                }
                else {
                    $jsdata=json_encode(array("fail"));
                    echo $jsdata;
                }
            }

            $conn = NULL;
            
        } catch (PDOException $e) {
            // return 503 if database error
            header('HTTP/1.1 503 Service Temporarily Unavailable');
            header('Status: 503 Service Temporarily Unavailable');
            header('Location: ./../503.php');        
        }
    }
?>
