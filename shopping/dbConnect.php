<?php
	$host = "localhost";
	$dbUser = "root";
	$dbPwd = "";
	$database = "syp_shopping_1";
	

	$connection = @mysqli_connect($host, $dbUser, $dbPwd, $database);
	if (!$connection)
		die("Error connecting to database");
     function listItems($itemNr,$name,$qty){
         global $connection;
         $sql= "SELECT $itemNr,$name,$qty FROM cartitems "

     }

?>