$(document).ready(function (e) {


$('#username').val("markus joppich");
$('#usermail').val("markus@joppich.info");
$('#template_name').val("testplate");


$('#user_template').val("<xml></xml>");

$("#importfile").on("click", function() {

$.ajax({
           url : $("#importfile_location").text(),
           dataType: "text",
           success : function (data) {
               $("#user_template").text(data);
           }
       });

});

$('#submit_template').submit( function(e) {

e.preventDefault();

var sUserTemplate = $('#user_template').val();
var bIsXML = false;

try {
var oXML = $.parseXML(sUserTemplate)
bIsXML = ($.isXMLDoc(oXML));

} catch(e) {

    bIsXML = false;
    alert(e);

}

if (!bIsXML)
    return;
    
var sTemplateName = $("#template_name").val();
var sTemplateType = $("#template_type").val();

var oFormData = new FormData(this);
oFormData.append("template", sUserTemplate);
oFormData.append("template_name", sTemplateName);
oFormData.append("template_type", sTemplateType);

$.ajax({
url: "ajax_php_file.php", // Url to which the request is send
type: "POST",             // Type of request to be send, called as method
data: oFormData, // Data sent to server, a set of key/value pairs (i.e. form fields and values)
contentType: false,       // The content type used when sending data to the server.
cache: false,             // To unable request pages to be cached
processData:false,        // To send DOMDocument or non processed data file it is set to false
success: function(data)   // A function to be called if request succeeds
{

$('#user_template').val(data);

}
});

});


});
/*
$.ajax({
url: "ajax_php_file.php", // Url to which the request is send
type: "POST",             // Type of request to be send, called as method
data: new FormData(this), // Data sent to server, a set of key/value pairs (i.e. form fields and values)
contentType: false,       // The content type used when sending data to the server.
cache: false,             // To unable request pages to be cached
processData:false,        // To send DOMDocument or non processed data file it is set to false
success: function(data)   // A function to be called if request succeeds
{


}
});


// Function to preview image after validation
$(function() {
$("#file").change(function() {
$("#message").empty(); // To remove the previous error message
var file = this.files[0];
var imagefile = file.type;
var match= ["image/jpeg","image/png","image/jpg"];
if(!((imagefile==match[0]) || (imagefile==match[1]) || (imagefile==match[2])))
{
$('#previewing').attr('src','noimage.png');
$("#message").html("<p id='error'>Please Select A valid Image File</p>"+"<h4>Note</h4>"+"<span id='error_message'>Only jpeg, jpg and png Images type allowed</span>");
return false;
}
else
{
var reader = new FileReader();
reader.onload = imageIsLoaded;
reader.readAsDataURL(this.files[0]);
}
});
});
function imageIsLoaded(e) {
$("#file").css("color","green");
$('#image_preview').css("display", "block");
$('#previewing').attr('src', e.target.result);
$('#previewing').attr('width', '250px');
$('#previewing').attr('height', '230px');
};
});

*/