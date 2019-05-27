<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
    <title>Sign in&up</title>
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
        $loginFlag = true;
        if(!isset($_SESSION['userName'])) // If session is not set then redirect to Login Page
        {
            $loginFlag = false;
        }
        else
        {
            $username = $_SESSION['userName'];
        }
    ?>

    <!-- NavMenu -->
    <header>
        <nav class="navbar navbar-expand-lg navbar-light bg-light">
            <a class="navbar-brand" href="#">myHotel</a>
            <button class="navbar-toggler" type="button" data-toggle="collapse" data-target="#navbarSupportedContent" aria-controls="navbarSupportedContent" aria-expanded="false" aria-label="Toggle navigation">
                <span class="navbar-toggler-icon"></span>
            </button>
            <div class="collapse navbar-collapse" id="navbarSupportedContent">
                <ul class="navbar-nav mr-auto">
                    <li class="nav-item">
                        <a class="nav-link" href="index.php">Home</a>
                    </li>
                    <li class="nav-item">
                        <a class="nav-link" href="book.php">Book Room</a>
                    </li>
                    <li class="nav-item">
                        <a class="nav-link" href="manage.php">Manage Book</a>
                    </li>
                </ul>
            </div>
        </nav>
    </header>

    <main role="main">
        <!-- Page Title -->
        <section class="jumbotron jumbotron-fluid text-center page-title">
            <div class="container">
                <h1 class="jumbotron-heading">Sign in / Sign up</h1>
                <p class="lead text-muted">Sign in with your acccount or create a new account!</p>
            </div>
        </section>
        <div class="login-register-area pt-100 pb-100">
            <div class="container">
                <div class="row">
                    <div class="col-lg-7 col-md-12 ml-auto mr-auto">
                        <div class="login-register-wrapper">
                            <div class="login-register-tab-list nav">
                                <a class="active" data-toggle="tab" href="#lg1">
                                    <h4> login </h4>
                                </a>
                                <a data-toggle="tab" href="#lg2">
                                    <h4> register </h4>
                                </a>
                            </div>
                            <div class="tab-content">
                                <div id="lg1" class="tab-pane active">
                                    <div class="login-form-container">
                                        <div class="login-register-form">
                                            <form action="./func/signin_post.php" method="post">
                                                <div class="form-group">
                                                    <label for="InputUsername">Username</label>
                                                    <input type="text" class="form-control textInput" id="InputUsername" placeholder="Enter username" name="username">
                                                </div>
                                                <div class="form-group">
                                                    <label for="InputPassword">Password</label>
                                                    <input type="password" class="form-control textInput" id="InputPassword" placeholder="Enter Password" name="password">
                                                </div>
                                                <div class="form-check">
                                                    <input class="form-check-input" type="checkbox" name="isStaff" id="staffSignin">
                                                    <label class="form-check-label" for="staffSignin">
                                                        Staff Signin
                                                    </label>
                                                </div>
                                                <button type="submit" class="btn btn-primary">Submit</button>
                                            </form>
                                        </div>
                                    </div>
                                </div>
                                <div id="lg2" class="tab-pane">
                                    <div class="login-form-container">
                                        <div class="login-register-form">
                                            <form action="./func/signup_post.php" method="post">
                                                <div id="usergroupInput" class="form-group">
                                                    <div class="form-check form-check-inline">
                                                        <input class="form-check-input" type="radio" name="inlineRadioOptions" id="customerRadio" value="customer" checked>
                                                        <label class="form-check-label" for="customerRadio">Customer</label>
                                                    </div>
                                                    <div class="form-check form-check-inline">
                                                        <input class="form-check-input" type="radio" name="inlineRadioOptions" id="staffRadio" value="staff">
                                                        <label class="form-check-label" for="staffRadio">Staff</label>
                                                    </div>
                                                </div>
                                                <div class="form-group">
                                                    <label for="InputUsername">Username</label>
                                                    <input type="text" class="form-control" id="InputUsername" placeholder="Enter username" name="username">
                                                </div>
                                                <div class="form-group">
                                                    <label for="InputPassword">Password</label>
                                                    <input type="password" class="form-control" id="InputPassword" placeholder="Enter Password" name="password">
                                                </div>
                                                <div class="form-group">
                                                    <label for="InputTruename">Truename</label>
                                                    <input type="text" class="form-control" id="InputTruename" placeholder="Name" name="truename">
                                                </div>
                                                <div class="form-group" id="InputStaffidDiv">
                                                    <label for="InputStaffid">Staff ID</label>
                                                    <input type="text" class="form-control" id="InputStaffid" placeholder="Staff ID" name="staffid">
                                                </div>
                                                <div class="form-group"  id="InputPassportDiv">
                                                    <label for="InputPassport">Passport</label>
                                                    <input type="text" class="form-control" id="InputPassport" placeholder="Passport ID" name="passport">
                                                </div>
                                                <div class="form-group">
                                                    <label for="InputTelephone">Telephone</label>
                                                    <input type="text" class="form-control" id="InputTelephone" placeholder="Phone Number" name="telephone">
                                                </div>
                                                <div class="form-group">
                                                    <label for="InputEmail">Email address</label>
                                                    <input type="email" class="form-control" id="InputEmail" placeholder="Email Address" name="email">
                                                </div>
                                                <button type="submit" class="btn btn-primary">Submit</button>
                                            </form>
                                        </div>
                                    </div>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </main>

    <footer class="text-muted">
        <div class="container text-center">
            <p><a href="index.php">myHotel</a> © 2019 All Right Reserved</p>
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
        // trick for register form
        $(document).ready(function() {
            $("#InputStaffidDiv").hide();
            $('input[name="inlineRadioOptions"]').click(function(){
                var value = $(this).val();
                console.log(value);
                if (value == "customer") {
                    $("#InputStaffidDiv").hide();
                    $("#InputPassportDiv").show();
                }
                else {
                    $("#InputPassportDiv").hide();
                    $("#InputStaffidDiv").show();
                }
            });
        })
    </script>    

    <style>
        .page-title {
            background-size: cover;
            background-repeat: no-repeat;
            background-image: url(img/background/page-title.jpg);
            background-color: rgba(255, 255, 255, 0.58);
            background-blend-mode: lighten;
        }
        .login-register-wrapper {
            margin-top: 30px;
            margin-bottom: 60px;
        }
        .login-register-wrapper .login-register-tab-list {
            justify-content: center;
            display: flex;
            -webkit-box-pack: center;
            margin-bottom: 40px;
        }
        .login-register-wrapper .login-register-tab-list a {
            position: relative;
            color: black;
        }
        .login-register-wrapper .login-register-tab-list a::before {
            background-color: #454545;
            bottom: 5px;
            content: "";
            height: 18px;
            margin: 0 auto;
            position: absolute;
            right: -2px;
            -webkit-transition: all 0.4s ease 0s;
            -o-transition: all 0.4s ease 0s;
            transition: all 0.4s ease 0s;
            width: 1px;
        }
        .login-register-wrapper .login-register-tab-list a h4 {
            font-size: 25px;
            font-weight: 700;
            margin: 0 20px;
            text-transform: capitalize;
            -webkit-transition: all 0.3s ease 0s;
            -o-transition: all 0.3s ease 0s;
            transition: all 0.3s ease 0s;
        }
        .login-register-wrapper .login-register-tab-list a:last-child::before {
            display: none;
        }
        .login-register-wrapper .login-register-tab-list a.active h4 {
            color: #17a2b8;
        }
        .login-form-container {
            background: transparent none repeat scroll 0 0;
            -webkit-box-shadow: 0 0 6px rgba(0, 0, 0, 0.1);
            box-shadow: 0 0 6px rgba(0, 0, 0, 0.1);
            padding: 80px;
            text-align: left;
        }

        .login-register-wrapper .login-form-container .login-register-form form .textInput {
            background-color: transparent;
            border: 1px solid #ebebeb;
            font-size: 14px;
            height: 50px;
            margin-bottom: 30px;
            padding: 0 15px;
            width: 100%;
        }
        .login-register-wrapper .login-form-container .login-register-form form .form-check {
            display: inline-block;
        }
        .login-register-wrapper .login-form-container .login-register-form form button {
            float: right;
        }
        .login-register-wrapper .login-form-container .login-register-form form #usergroupInput {
            justify-content: center;
            display: flex;
        }
    </style>

    <!-- Logout Modal -->
    <div class="modal fade" id="noAccessModal" tabindex="-1" role="dialog" aria-labelledby="noAccessModalLabel" aria-hidden="true" data-backdrop="static" data-keyboard="false">
        <div class="modal-dialog" role="document">
            <div class="modal-content">
                <div class="modal-header">
                    <h5 class="modal-title" id="noAccessModalLabel">Already Login</h5>
                </div>
                <div class="modal-body">
                    You have already sign in yet, Please sign out first if you want change a account.
                </div>
                <div class="modal-footer">
                    <a class="btn btn-primary" href="index.php" role="button">Back to home page</a>
                    <a class="btn btn-primary" href="./func/signout_post.php" role="button">Click to sign out</a>
                </div>
            </div>
        </div>
    </div>

    <!-- Process Error Modal -->
    <div class="modal fade" id="errorModal" tabindex="-1" role="dialog" aria-labelledby="errorModalLabel" aria-hidden="true">
        <div class="modal-dialog" role="document">
            <div class="modal-content">
                <div class="modal-header">
                    <h5 class="modal-title" id="errorModalLabel">No Room</h5>
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close">
                    <span aria-hidden="true">&times;</span>
                    </button>
                </div>
                <div class="modal-body">
                    We have no more rooms availiable for your current choice, please change your choice and book again.
                </div>
            </div>
        </div>
    </div>

    <?php
        // handle failed get
        if ($loginFlag == true) {
            echo "<script>$('#noAccessModal').modal('show')</script>";
        }
        else {
            if(isset($_GET['signin'])) {
                echo "<script>$('#errorModalLabel').html('Sign in Failed');$('#errorModal .modal-body:first').html('Please check your username and password');$('#errorModal').modal('show')</script>";
            };
            if(isset($_GET['signup'])) {
                echo "<script>$('#errorModalLabel').html('Sign up Failed');$('#errorModal .modal-body:first').html('<strong>You can try:</strong><br>1.Fill up all labels in form<br>2.Change username');$('#errorModal').modal('show')</script>";
            };
        }
    ?>
</body>