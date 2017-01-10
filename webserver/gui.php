<html>
<head>
<title>bioGUI: template submission</title>
<link rel="stylesheet" href="styles.css" />
<link href='http://fonts.googleapis.com/css?family=Roboto+Condensed|Open+Sans+Condensed:300' rel='stylesheet' type='text/css'>
<script src="http://ajax.googleapis.com/ajax/libs/jquery/1.11.1/jquery.min.js"></script>
<script src="script.js"></script>
</head>
<body>
<div class="main">
<h1>bioGUI: template submission</h1><br/>

<form id="submit_template" action="" method="post" enctype="multipart/form-data">
<div id="submission_user">

<table>
<tr>
<td><label>Your name</label>
</td>
<td><input type="input" name="username" id="username" required/>
</td>
</tr>
<tr>
<td><label>Your eMail Address</label>
</td>
<td><input type="email" name="usermail" id="usermail" required/>
</td>
</tr>
<tr></tr>
<tr></tr>

<tr>
<td><label>Your template name:</label></td>
<td><input type="input" id="template_name" required/></td>
</tr>

<tr>
<td><label>Select a template type:</label></td>
<td>

  <input type="radio" name="template_type" id="template_type" value="0" checked> Template<br>
  <input type="radio" name="template_type" id="template_type" value="1"> Install Template

</td>
</tr>


<tr>
<td><label>Your template code:</label></td>
<td><textarea cols="40" rows="10" id="user_template" placeholder="<xml></xml>" required></textarea></td>
</tr>

<tr>
<td></td>
<td>Import template from file:</br>
<input type="file" id="importfile_location" /></br>
<input type="button" id="importfile" value="Import File" />
</td>
</tr>
</table>

<br/>
<input type="submit" value="Submit Template"/>
</div>
</body>
</html>