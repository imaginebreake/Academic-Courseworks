<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
    <title>Home</title>
    <!-- Font Awesome -->
    <link rel="stylesheet" href="css/fontawesome/all.css">
    <!-- Bootstrap core CSS -->
    <link href="css/bootstrap.min.css" rel="stylesheet" type="text/css" />
    <!-- GIJGO -->
    <link href="css/gijgo.min.css" rel="stylesheet" type="text/css" />
    <!-- DataTable -->
    <link href="css/dataTables.bootstrap4.min.css" rel="stylesheet" type="text/css" />

    <!-- Your custom styles (optional) -->
    <link href="css/style.css" rel="stylesheet">
</head>

<body>
    <?php
        // check login status
        session_start();
        $username = "";
        $usergroup = "";
        $loginFlag = true;
        if(!isset($_SESSION['userName']))
        {
            $loginFlag = false;
        }
        else
        {
            $usergroup = $_SESSION['userGroup'];
            $username = $_SESSION['userName'];
        }
    ?>
    
    <!-- NavMenu -->
    <header>
        <nav class="navbar fixed-top navbar-expand-lg navbar-light bg-light">
            <a class="navbar-brand" href="#">SunnyIsle</a>
            <button class="navbar-toggler" type="button" data-toggle="collapse" data-target="#navbarSupportedContent" aria-controls="navbarSupportedContent" aria-expanded="false" aria-label="Toggle navigation">
                <span class="navbar-toggler-icon"></span>
            </button>
            <div class="collapse navbar-collapse" id="navbarSupportedContent">
                <ul class="navbar-nav mr-auto">
                    <li class="nav-item active">
                        <a class="nav-link" href="index.php">Home<span class="sr-only">(current)</span></a>
                    </li>
                    <li class="nav-item">
                        <a class="nav-link" href="book.php">Book Room</a>
                    </li>
                    <li class="nav-item">
                        <a class="nav-link" href="manage.php">Manage Book</a>
                    </li>
                </ul>
                <span class="navbar-text" id="navUserName">
                    Welcome, <?php echo $username?>&nbsp;&nbsp;
                </span>
                <a href="./func/signout_post.php" id="navSignOut" class="btn btn-primary m-2 my-sm-0">Sign Out</a>
                <a href="signin_up.php" id="navSignIn" class="btn btn-primary m-2 my-sm-0">Sign In</a>
                <a href="signin_up.php" id="navSignUp" class="btn btn-primary m-2 my-sm-0">Sign Up</a>
            </div>
        </nav>
    </header>

    <main role="main"  style="background-image: url('img/background/home.jpg'); background-repeat: no-repeat; background-size: cover; background-position: center center;">
        <!-- main content -->
        <div id="mainContent">
            <h1 style="color: white; margin-bottom: 20px">Welcome to SunnyIsle</h1>
            <a href="book.php" class="btn btn-outline-info btn-light btn-lg" role="button">Book Room Now</a>
        </div>

    </main>

    <footer class="text-muted">
        <div class="container text-center">
            <p><a href="index.php">SunnyIsle</a> © 2019 All Right Reserved</p>
        </div>
    </footer>

    <a href="javascript:" id="return-to-top"><i class="fas fa-arrow-up"></i></a>

    <!-- SCRIPTS -->
    <!-- JQuery -->
    <script type="text/javascript" src="js/jquery-3.3.1.min.js"></script>
    <!-- Bootstrap tooltips -->
    <script type="text/javascript" src="js/popper.min.js"></script>
    <!-- Bootstrap core JavaScript -->
    <script type="text/javascript" src="js/bootstrap.min.js"></script>
    <!-- GIJGO JavaScript -->
    <script type="text/javascript" src="js/gijgo.min.js"></script>
    <!-- JqueryDataTable JavaScript -->
    <script type="text/javascript" src="js/jquery.dataTables.min.js"></script>
    <!-- JqueryDataTable JavaScript -->
    <script type="text/javascript" src="js/dataTables.bootstrap4.min.js"></script>

    <script type="text/javascript" src="js/backtotop.js"></script>

    <script>
        // diffrent nav for different login status
        function showNavInfo() {
            var username = "<?php echo $username ?>";
            console.log(username);
            if (username != "") {
                $("#navSignIn").css('display','none'); 
                $("#navSignUp").css('display','none'); 
            }
            else {
                $("#navUserName").css('display','none'); 
                $("#navSignOut").css('display','none');
            }
        }
        showNavInfo.call();
        // change link and msg in button
        function changeBTNInfo() {
            if ("<?php echo $username ?>" != "") {
                var usergroup = "<?php echo $usergroup ?>";
                // manage room for staff
                if (usergroup == "staff") {
                    $("#mainContent a:first").attr("href","manage.php");
                    $("#mainContent a:first").html("Manage Rooms");
                }
            }
            else {
                // sign in for no login
                $("#mainContent a:first").attr("href","signin_up.php");
                $("#mainContent a:first").html("Sign In First");
            }
        }
        changeBTNInfo.call();
    </script>

    <style>
        footer {
            position: fixed;
            left: 0;
            right: 0;
            bottom: 0
        }
        main {
            height: 100vh;
            display: -ms-flexbox;
            display: flex;
            -ms-flex-align: center;
            align-items: center;
        }
        #mainContent {
            margin-left: auto;
            margin-right: auto;
            text-align: center
        }
    </style>
</body>
</html>