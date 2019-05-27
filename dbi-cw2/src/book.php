<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
    <title>Book</title>
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
        // check login status and display different webpage
        session_start();
        $username = "";
        $loginFlag = true;
        $usergroup = "";
        if(!isset($_SESSION['userName']))
        {
            $loginFlag = false;
        }
        else {
            if ($_SESSION['userGroup'] === "staff")
            {
                $loginFlag = false;
            }
            $username = $_SESSION['userName'];
            $usergroup = $_SESSION['userGroup'];
        }
    ?>
    <!-- No Room Modal -->
    <div class="modal fade" id="noRoomModal" tabindex="-1" role="dialog" aria-labelledby="noRoomModalLabel" aria-hidden="true">
        <div class="modal-dialog" role="document">
            <div class="modal-content">
                <div class="modal-header">
                    <h5 class="modal-title" id="noRoomModalLabel">No Room</h5>
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
    <!-- NavMenu -->
    <header>
        <nav class="navbar navbar-expand-lg navbar-light bg-light">
            <a class="navbar-brand" href="#">SunnyIsle</a>
            <button class="navbar-toggler" type="button" data-toggle="collapse" data-target="#navbarSupportedContent" aria-controls="navbarSupportedContent" aria-expanded="false" aria-label="Toggle navigation">
                <span class="navbar-toggler-icon"></span>
            </button>
            <div class="collapse navbar-collapse" id="navbarSupportedContent">
                <ul class="navbar-nav mr-auto">
                    <li class="nav-item">
                        <a class="nav-link" href="index.php">Home</a>
                    </li>
                    <li class="nav-item active">
                        <a class="nav-link" href="book.php">Book Room<span class="sr-only">(current)</span></a>
                    </li>
                    <li class="nav-item">
                        <a class="nav-link" href="manage.php">Manage Book</a>
                    </li>
                </ul>
                <span class="navbar-text">
                    Welcome, <?php echo $username?>&nbsp;&nbsp;
                </span>
                <a href="./func/signout_post.php" class="btn btn-primary my-2 my-sm-0">Sign Out</a>
            </div>
        </nav>
    </header>

    <main role="main">
        <!-- Page Title -->
        <section class="jumbotron jumbotron-fluid text-center page-title">
            <div class="container">
                <h1 class="jumbotron-heading">Book Your Room</h1>
                <p class="lead text-muted">Check availiable rooms and submit a book.</p>
            </div>
        </section>
        <!-- Room Types -->
        <div class="container" id="roomInfoContainer">
            <div class="row">
                <div class="col-lg-3 col-md-6 col-sm-12">
                    <div class="card-group">
                        <div class="card">
                            <img src="img/single_S.jpg" class="card-img-top" alt="...">
                            <div class="card-body">
                                <h5 class="card-title">Small Single Room</h5>
                                <p class="card-text"></p>
                            </div>
                        </div>
                    </div>
                </div>
                <div class="col-lg-3 col-md-6 col-sm-12">
                    <div class="card-group">
                        <div class="card">
                            <img src="img/single_L.jpg" class="card-img-top" alt="...">
                            <div class="card-body">
                                <h5 class="card-title">Large Single Room</h5>
                                <p class="card-text">

                                </p>
                            </div> 
                        </div>                          
                    </div>
                </div>
                <div class="col-lg-3 col-md-6 col-sm-12">
                    <div class="card-group">
                        <div class="card">
                            <img src="img/double.jpg" class="card-img-top" alt="...">
                            <div class="card-body">
                                <h5 class="card-title">Large Double Room</h5>
                                <p class="card-text"></p>
                            </div>
                        </div>
                    </div>
                </div>
                <div class="col-lg-3 col-md-6 col-sm-12">
                    <div class="card-group">
                        <div class="card">
                            <img src="img/vip.jpg" class="card-img-top" alt="...">
                            <div class="card-body">
                                <h5 class="card-title">VIP Room</h5>
                                <p class="card-text"></p>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
            <br>
        </div>
        <!-- Room Query -->
        <div class="container" style="margin-bottom: 20px">
            <div class="row">
                <div class="col-9">
                    <div class="card-group">
                        <div class="card">           
                            <h5 class="card-header">Room Position Instruction (X is for number of floor)</h5>
                            <div class="card-body" style="padding: 0px">
                                <table class="table table-bordered">
                                    <tbody>
                                        <tr class="text-center">
                                            <td><strong>Large Double bed</strong><br>Room X01</td>
                                            <td><strong>Large Double bed</strong><br>Room X02</td>
                                            <td><strong>Large Single bed</strong><br>Room X03</td>
                                            <td><strong>Large Single bed</strong><br>Room X04</td>
                                            <td><strong>Small Single bed</strong><br>Room X05</td>
                                        </tr>
                                        <tr style="height: 1em;">
                                        </tr>
                                        <tr class="text-center">
                                            <td rowspan="2"><strong>VIP Room</strong><br>Room X13</td>
                                            <td colspan="3" rowspan="2" id="lobbyCell">Stairs & Lobby</td>
                                            <td><strong>Small Single bed</strong><br>Room X06</td>
                                        </tr>
                                        <tr class="text-center">
                                            <td><strong>Small Single bed</strong><br>Room X07</td>
                                        </tr>
                                        </tr>
                                            <tr style="height: 1em;">
                                        </tr>
                                        <tr class="text-center">
                                            <td><strong>Large Double bed</strong><br>Room X12</td>
                                            <td><strong>Large Double bed</strong><br>Room X11</td>
                                            <td><strong>Large Single bed</strong><br>Room X10</td>
                                            <td><strong>Large Single bed</strong><br>Room X09</td>
                                            <td><strong>Small Single bed</strong><br>Room X08</td>
                                        </tr>
                                    </tbody>
                                </table>
                            </div>
                        </div>
                    </div>
                </div>
                <div class="col-3">
                    <form id="checkRoomForm" style="height: 100%" onsubmit="return subCheckRoom();">
                        <div class="dateRangePicker"  style="height: 30%">
                            <label for="startDate">Start Date:</label> 
                            <input name="startdate" id="startDate" class="DateRangePicker"/>
                        </div>
                        <div class="dateRangePicker"  style="height: 30%">
                            <label for="endDate">End Date:</label> 
                            <input name="enddate" id="endDate" class="DateRangePicker"/>
                        </div>
                        <div style="height: 30%">
                            <label for="roomtypeSelector">Room Type:</label> 
                            <select class="form-control" id="roomtypeSelector" name="roomtype">
                                <option value="single_s">Small Single Room</option>
                                <option value="single_l">Large Single Room</option>
                                <option value="double" selected="selected">Large Double Room</option>
                                <option value="vip">VIP Room</option>
                            </select>
                        </div>
                        <div class="justify-content-end align-items-end" style="height: 10%">
                            <button class="btn btn-primary" style="width: 100%">Check availiable rooms</button>
                        </div>
                    </form>
                </div>
            </div>
        </div>
        <!-- Book -->
        <div class="container" id="roomSelector">
            <div class="row" style="margin-top: 20px">
                <div class="col-12">
                    <div class="card">
                        <div class="card-header"  id="bookinfo">
                        </div>
                        <div class="card-body">
                            <h5 class="card-title">
                            Choose your ideal room!
                            </h5>
                            <p><strong>Tips:&nbsp;</strong>Last Row of Table is For Fast Selection</p>
                            <table id="roomTable" class="table table-striped table-bordered" style="width:100%">
                                <thead>
                                    <tr>
                                        <th>Room ID</th>
                                        <th>Room Floor</th>
                                        <th>Room Position</th>
                                    </tr>
                                </thead>
                                <tbody>
                                </tbody>
                                <tfoot>
                                    <tr>
                                        <th>Room ID</th>
                                        <th>Room Floor</th>
                                        <th>Room Position</th>
                                    </tr>
                                </tfoot>
                            </table>
                            <hr>
                            <h5>Enter your ideal room!</h5>
                            <form class="form-inline" id="submitRoomChoice">
                                <div class="form-group mb-2">
                                    <input type="text" class="form-control" id="inputRoomNumber" placeholder="Room ID">
                                </div>
                                <button type="submit" id="normalChooseBTN" class="btn btn-primary mx-sm-3 mb-2">I want this room</button>
                                <button type="submit" id="randomChooseBTN" class="btn btn-secondary mb-2">Choose one for me</button>
                            </form>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </main>

    <footer class="text-muted">
        <div class="container text-center">
            <p><a href="#">SunnyIsle</a> © 2019 All Right Reserved</p>
        </div>
    </footer>

    <!-- scroll to top -->
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

    <script type="text/javascript" src="js/book.js"></script>
    <script type="text/javascript" src="js/backtotop.js"></script>

    <script>
        var roomTableData;
        var user_name = "<?php echo $username;?>";
    </script>

    <style>
        .dateRangePicker > .gj-datepicker{
            width: 100%;
            margin-bottom: 1rem;
        }
        #lobbyCell {
            border-top-width: 0px;
            border-bottom-width: 0px;
            font-size: 200%;
            font-weight: bold;
        }
        .table > tbody > tr > td {
            vertical-align: middle;
        }
        .table {
            margin: 0px;
        }
        .page-title {
            background-size: cover;
            background-repeat: no-repeat;
            background-image: url(img/background/page-title.jpg);
            background-color: rgba(255, 255, 255, 0.58);
            background-blend-mode: lighten;
        }
        
    </style>

    <!-- Login Modal -->
    <div class="modal fade" id="noAccessModal" tabindex="-1" role="dialog" aria-labelledby="noAccessModalLabel" aria-hidden="true" data-backdrop="static" data-keyboard="false">
        <div class="modal-dialog" role="document">
            <div class="modal-content">
                <div class="modal-header">
                    <h5 class="modal-title" id="noAccessModalLabel">No Sign In</h5>
                </div>
                <div class="modal-body">
                    You are not sign in yet, Please sign in first.
                </div>
                <div class="modal-footer">
                    <a class="btn btn-primary" href="signin_up.php#lg1" role="button">Click to Sign in</a>
                </div>
            </div>
        </div>
    </div>

    <!-- Book Info Modal -->
    <div class="modal fade" id="bookInfoModal" tabindex="-1" role="dialog" aria-labelledby="bookInfoModalLabel" aria-hidden="true" data-backdrop="static" data-keyboard="false">
        <div class="modal-dialog" role="document">
            <div class="modal-content">
                <div class="modal-header">
                    <h5 class="modal-title" id="bookInfoModalLabel">Your Book is Approved</h5>
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close">
                    <span aria-hidden="true">&times;</span>
                    </button>
                </div>
                <div class="modal-body">
                    <table class="table table-borderless">
                        <tr>
                            <th>User</th>
                            <td>1</td>
                        </tr>
                        <tr>
                            <th>Start Date</th>
                            <td>2</td>
                        </tr>
                        <tr>
                            <th>End Date</th>
                            <td>3</td>
                        </tr>
                        <tr>
                            <th>Room Number</th>
                            <td>4</td>
                        </tr>
                    </table>
                </div>
            </div>
        </div>
    </div>

    <?php 
        // show modals
        if ($loginFlag == false) {
            // staff - using staff account
            if ($usergroup == "staff") {
                echo "<script>$('#noAccessModalLabel').html('Using staff account');$('#noAccessModal .modal-body:first').html('Please logout and use a customer account');$('#noAccessModal a:first').attr('href','func/signout_post.php');$('#noAccessModal a:first').html('Log Out');$('#noAccessModal').modal('show')</script>";
            }
            // no login
            else {
                echo "<script>$('#noAccessModal').modal('show')</script>";
            }
        }
    ?>
</body>

</html>
