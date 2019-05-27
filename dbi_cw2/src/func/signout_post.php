<?php
    session_start();

    // initialize seesion array
    $_SESSION = array();

    // destory cookies
    if (ini_get("session.use_cookies")) {
        $params = session_get_cookie_params();
        setcookie(session_name(), '', time() - 42000,
            $params["path"], $params["domain"],
            $params["secure"], $params["httponly"]
        );
    }

    // destory session
    session_destroy();

    // return to index page
    header('Location: ./../index.php');
?>