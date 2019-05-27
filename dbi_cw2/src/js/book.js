// set up dake picker
$("#roomSelector").css('display','none'); 
var today = new Date(new Date().getFullYear(), new Date().getMonth(), new Date().getDate());
$('#startDate').datepicker({
    uiLibrary: 'bootstrap4',
    iconsLibrary: 'fontawesome',
    minDate: today,
    format: 'yyyy-mm-dd',
    maxDate: function () {
        return $('#endDate').val();
    }
});
$('#endDate').datepicker({
    uiLibrary: 'bootstrap4',
    iconsLibrary: 'fontawesome',
    format: 'yyyy-mm-dd',
    minDate: function () {
        return $('#startDate').val();
    }
});

// function for room check form
function subCheckRoom() {
    // data for get request
    data = "startdate="+$('#startDate').datepicker().value()+"&enddate="+$('#endDate').datepicker().value()+"&roomtype="+$('#roomtypeSelector option:selected').val();
    // show modal if datepicker value invalid
    if (($('#startDate').datepicker().value().length < 1) || ($('#endDate').datepicker().value().length < 1)) {
        $('#noRoomModal').modal('show');
        return false;
    }
    // ajax for checkroom
    $.ajax({  
        cache: true,  
        type: "GET",  
        url:"func/checkroom_get.php?",  
        data: data,
        success: function(msg) {
            displayRooms(msg);
        },
    });
    return false; 
}

// display rooms after ajax
function displayRooms(msg) {
    var rooms_raw = JSON.parse(msg);
    // show modal if there is no room left
    if (rooms_raw.length < 1) {
        $('#noRoomModal').modal('show');
    }
    else {
        // reintilize data table
        if ($("#roomSelector").css('display') == 'block') {
            $("#roomTable").dataTable().fnDestroy();
            $("#roomSelector tbody").html("")
        }
        // roominfo for display on the choice card
        var username = "<strong>User: </strong>" + user_name + " ";
        var roominfo = "<strong>DateRange: </strong>" + $('#startDate').datepicker().value().replace(/-/g,"/")+"-"+$('#endDate').datepicker().value().replace(/-/g,"/")+" <strong>Room: </strong>"+$('#roomtypeSelector option:selected').val();
        var bookinfo = username + roominfo;
        $("#bookinfo").html(bookinfo);

        // clean data and display
        roomTableData = new Array();
        // console.log(rooms_raw);
        for(var i=0,len=rooms_raw.length;i<len;i++) {
            element = rooms_raw[i];
            room_number = element["room_number"];
            if (room_number.length == 3) {
                room_floor = room_number.slice(0,1);
                room_pos = room_number.slice(1,3);
            }
            else {
                room_floor = room_number.slice(0,2);
                room_pos = room_number.slice(2,4);
            }
            roomTableData.push({room_number,room_floor,room_pos});
            trString = '<tr>'+'<th>'+room_number+'</th>'+'<th>'+room_floor+'</th>'+'<th>'+room_pos+'</th>'+'</tr>';
            // console.log(trString);
            $('#roomTable > tbody:last').append('<tr>'+'<th>'+room_number+'</th>'+'<th>'+room_floor+'</th>'+'<th>'+room_pos+'</th>'+'</tr>');
        };
        // intilize table
        intiTable.call();
        $("#roomSelector").css('display','block');
    }
}

function intiTable() {
    $('#roomTable').DataTable( {
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

function getRandomInt(max) {
    return Math.floor(Math.random() * Math.floor(max));
}

$(document).ready(function() {
    // bind to choice button
    $("#submitRoomChoice button").click(function(event){
        // prevent form action
        event.preventDefault();
        var roomFlag = false;
        var roomIDs = new Array();
        for (var i=0;i<roomTableData.length;i++) {
            roomIDs.push(roomTableData[i]["room_number"]);
        }
        roomID = "";
        
        // check is room nuber valid
        if ($(this).attr("id") == "normalChooseBTN")
        {
            roomID = $("#inputRoomNumber").val();
            if (roomIDs.indexOf(roomID) == -1) {
                alert("Please Enter Correct Room ID");
                return false;
            }
        }
        // random select a room
        else
        {
            roomID = roomIDs[getRandomInt(roomIDs.length)]
        }

        // set up post data
        postData = {
            username: user_name,
            roomID: roomID,
            startdate: $('#startDate').datepicker().value(),
            enddate: $('#endDate').datepicker().value(),
        }

        // post book
        // console.log(postData);
        $.ajax({
            type: "POST",
            url: "func/bookroom_post.php",
            data: postData,
            dataType:'json',
            success: function(data) {
                // handle return data and show book info
                // console.log(data);
                $("#bookInfoModal td").each(function(i,item){
                    switch (i) {
                        case 0 :
                            $(item).html(data['userName']);
                            break;
                        case 1 :
                            $(item).html(data['startDate']);
                            break;
                        case 2 :
                            $(item).html(data['endDate']);
                            break;
                        case 3 :
                            $(item).html(data['roomNumber']);
                            break;
                    }
                });
                $('#bookInfoModal').modal('show')
            }
        });
        // refresh room table
        subCheckRoom.call();
    });
} );