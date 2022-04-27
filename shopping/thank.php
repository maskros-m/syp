<?php
 session_start();
 require_once 'dbTools.php';
 require_once 'dbAccess.php';
 //titel
 if ($_SESSION['gender']=='Frau'){
    $titel = "Liebe Frau " ;
 }
 else $titel="Lieber Herr ";
 //insert Table orders
 $id= $_SESSION['id'];
 $d= date("Y-m-d");
 $t=date("H:i:s");

 
$sql= "SELECT ROUND(SUM(totalPrice), 2) as s FROM cartitems";
$result= mysqli_query($con,$sql); 
if(mysqli_num_rows($result) >0){
   $row=mysqli_fetch_assoc($result);}
$s=$row['s'];

if($s < 30)
    $ship = 1;
else $ship = 2;

global $con;
$sql ="INSERT INTO `orders` ( `totalPrice`,`dateOrdered`, `timeOrdered`, `customerId`, `shippingCostId`) VALUES ('$s','$d','$t',$id,$ship)";
$result= mysqli_query($con,$sql);
$sql = "SELECT * FROM orders ORDER BY id DESC LIMIT 1 ";
$result = mysqli_query($con,$sql);
if(mysqli_num_rows($result) >0){
    $row=mysqli_fetch_assoc($result);}
 $idOrder=$row['id'];
 $dateOrder=$row['dateOrdered'];
 

//insert from cartItems to orderedItems
$sql= "INSERT INTO orderedItems (productId,Quantity,purchased_amount) SELECT productId,Quantity,totalPrice FROM cartItems";
$result= mysqli_query($con,$sql); 
$sql="UPDATE orderedItems SET orderId = $idOrder where orderId is null ";
mysqli_query($con,$sql); 

//delete all Data from cartitems
$sql = "DELETE FROM cartitems";
mysqli_query($con,$sql);


?>
<html>
    <head>
		<link rel="stylesheet" href="general-styling.css" />
        <link rel = "stylesheet" href="thank.css" />
        <meta charset="utf8"/>
        <title>Thank</title>
    </head>
    <body>
		<div id="wrapper" class="row">
			
			<div id="sideimg"><img src="images/flowerbg.png"></div>
            
            <div id="loginwrapper">
				
				<div id="container" class="col">
        
      
				<p><?php echo $titel. $_SESSION['lastName'];?>,
				</p>
				<p>
					vielen Dank für Ihre Bestellung. Diese hat soeben unsere Versandabteilung in Frankreich verlassen.<br>
					Die Sendung sollte nun in den kommenden 4-5 Werktagen bei Ihnen eintreffen.<br>
					Unser Logistikpartner, die Österreichische Post AG, bringt Ihnen Ihre Produkte direkt zu der von Ihnen angegebenen Adresse.
				</p>
				<p>
					<h3>LIEFERADRESSE</h3>
					<?php 
						echo $_SESSION['gender']." ".$_SESSION['firstName']." ".$_SESSION['lastName']."<br>"; 
						echo $_SESSION['street']." ".$_SESSION['housnr']."<br>"; 
						echo $_SESSION['plz']." ".$_SESSION['ort']."<br>";            
					?> 
				</p>
				<p>
					<h3>BESTELLUNGSÜBERSICHT</h3>
					Kundennummer : <?php echo $_SESSION['id']; ?><br>
					Bestell-Datum : <?php echo $dateOrder; ?><br>
					Bestellnummer : <?php echo $idOrder; ?><br>
				</p>
				<p>
					<b>Wir sind für Sie da</b><br>
					Bei Fragen steht Ihnen unser Kundenservice per E-Mail oder auch gerne per Telefon unter der Rufnummer 01 - 9 622 422 von Montag-Freitag 7.30 - 19.00 Uhr und Samstag 8.00 - 18.00 Uhr zur Verfügung. (Anruf zu Ihrem jeweiligen Festnetz- oder Mobilfunk Tarif).
				</p>
				<p>
					Herzliche Grüße <br>
					Ihr Mily Team
				</p>
				<a href="einkauf.php" class="bigbtn">WEITER KAUFEN</a>
				
				<div id="credit" class="row center hasmargintop">
					<div class="one-quarter-box center row topborder">
					<center><p class="smallprint">A php project by Ly Tran & Mi Do<br> 2AKIFT - April 2022</p></center>
				</div>
			</div>

        </div>
		
		
        
    </body>
</html>