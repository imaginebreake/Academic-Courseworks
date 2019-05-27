<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
    <title>Manage</title>
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
        if (!isset($_SESSION['userName']))
        {
            $loginFlag = false;
        }

        if ($loginFlag == false)
        {
            // 404 for customer/nologin
            header('HTTP/1.1 404 Not Found'); 
            header("status: 404 Not Found"); 
            include('404.php');
            exit();
        };
    ?>

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
                    <li class="nav-item">
                        <a class="nav-link" href="book.php">Book Room<span class="sr-only">(current)</span></a>
                    </li>
                    <li class="nav-item active">
                        <a class="nav-link" href="manage.php">Manage Book<span class="sr-only">(current)</span></a>
                    </li>
                </ul>
            </div>
        </nav>
    </header>

    <main role="main">
        <!-- select book -->
        <div class="container" id="bookSelector">
            <div class="row" style="margin-top: 20px">
                <div class="col-12">
                    <div class="card">
                        <div class="card-header"  id="bookinfo">Book Manager
                        </div>
                        <div class="card-body">
                            <h5 class="card-title">
                            </h5>
                            <table id="bookTable" class="table table-striped table-bordered" style="width:100%">
                                <thead>
                                    <tr>
                                        <th>Book ID</th>
                                        <th>Room ID</th>
                                        <th>Room Type</th>
                                        <th>Status</th>
                                        <th>Date</th>
                                        <th>Customer</th>
                                    </tr>
                                </thead>
                                <tbody>
                                </tbody>
                                <tfoot>
                                    <tr>
                                        <th>Book ID</th>
                                        <th>Room ID</th>
                                        <th>Room Type</th>
                                        <th>Status</th>
                                        <th>Date</th>
                                        <th>Customer</th>
                                    </tr>
                                </tfoot>
                            </table>
                            <hr>
                            <form class="form-inline" id="submitBookChoice">
                                <div class="form-group mb-2">
                                    <input type="text" class="form-control" id="inputBookNumber" placeholder="Book ID">
                                </div>
                                <button type="submit" id="checkInBTN" class="btn btn-outline-primary ml-2 mb-2">Check In</button>
                                <button type="submit" id="checkOutBTN" class="btn btn-outline-success ml-1 mb-2">Check Out</button>
                                <button type="submit" id="cancelBTN" class="btn btn-outline-info ml-1 mb-2">Cancel</button>
                            </form>
                        </div>
                    </div>
                </div>
            </div>
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
        // closure function for table status
        var isInti = (function() {
                var intiFlag = false;
                function cFlag() {
                    intiFlag = !intiFlag;
                }
                return {
                    flag: function() {
                        return intiFlag;
                    },
                    changeFlag: function() {
                        cFlag();
                    }
                }
        })();

        // intilize table
        function intiTable() {
            $('#bookTable').DataTable( {
                initComplete: function () {
                    this.api().columns().every( function () {
                        var column = this;
                        var select = $('<select><option value=""></option></select>')
                            .appendTo( $(column.footer()).empty() )
                            .on( 'change', function () {
                                var val = $.fn.dataTable.util.escapeRegex(
                                    $(this).val()
                                );

                                column
                                    .search( val ? '^'+val+'$' : '', true, false )
                                    .draw();
                            } );

                        column.data().unique().sort().each( function ( d, j ) {
                            select.append( '<option value="'+d+'">'+d+'</option>' )
                        } );
                    } );
                }
            } );
        };

        var TableData = Object();

        // refresh book when data is update
        function refreshBook() {
            if (isInti.flag() == false) {
                isInti.changeFlag();
            }
            else {
                $("#bookTable").dataTable().fnDestroy();
            }
            $("#bookSelector tbody").html("");
            // ajax request for book
            $.ajax({
                url: "func/getbook.php",
                dataType:'json',
                success: function(data) {
                    // prepare data in table
                    // console.log(data);
                    TableData = data;
                    for(var i=0,len=data.length;i<len;i++) {

                        element = data[i];
                        bookID = element['bID'];
                        roomID = element['room_number'];
                        roomType = element['room_type'];
                        status = element['status'];
                        if (status == "0") {
                            status = "book";
                        }
                        else if (status == "1") {
                            status = "check in"
                        }

                        date  = element['startDate'].replace(/-/g,"/") + "-" + element['endDate'].replace(/-/g,"/");
                        customer  = element['username'];

                        trString = '<tr>'+'<td>'+bookID+'</td>'+'<td>'+roomID+'</td>'+'<td>'+roomType+'</td>'+'<td>'+status+'</td>'+'<td>'+date+'</td>'+'<td>'+customer+'</td>'+'</tr>';

                        // console.log(trString);

                        $('#bookTable > tbody:last').append(trString);
                    };
                    intiTable();
                }
            });
        }

        $(document).ready(function() {
            if ("<?php echo $_SESSION['userGroup']?>" == "customer") {
                $("#checkInBTN").remove();
                $("#checkOutBTN").remove();
            }
            refreshBook.call();
            // bind submit button and handle book manage
            $("#submitBookChoice button").click(function(event){
                event.preventDefault();
                var data = TableData;
                var bookFlag = false;
                var bookIndex = -1;
                var bookID = $("#inputBookNumber").val();

                for (i = 0; i < data.length; i++)
                {
                    if (data[i]['bID'] == bookID)
                    {
                        bookFlag = true;
                        bookIndex = i;
                        break;
                    }
                }

                // check whether book is invalid
                if (bookFlag == false) {
                    alert("Book ID is incorrect!")
                    return false;
                }

                // handle opertaion
                opt = -1;
                if ($(this).attr("id") == "checkInBTN") {
                    opt = 1;
                }
                else if ($(this).attr("id") == "checkOutBTN") {
                    opt = 2;
                }
                else if ($(this).attr("id") == "cancelBTN") {
                    opt = -1;
                }

                // alert if opertaion illegal
                if ((opt == 1) && (data[bookIndex]["status"] == 1)) {
                    alert("You can not check in a book which is already checked in.");
                    return false;
                }
                else if ((opt == 2) && (data[bookIndex]["status"] == 0)) {
                    alert("You can not check out a book which is not checked in yet.");
                    return false;
                }
                
                postData = {
                    bID: bookID,
                    opt: opt,
                }

                // console.log(postData);

                // update book
                $.ajax({
                    type: "POST",
                    url: "func/managebook_post.php",
                    data: postData,
                    dataType:'json',
                    success: function(data) {
                        refreshBook.call();
                    }
                });
            });
        })
    </script>    

    <style>
        #bookSelector {
            margin-bottom: 30px;
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


    <?php

    ?>
</body>

