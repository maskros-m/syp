<?php
require_once 'dbTools.php';
require_once 'dbAccess.php';

session_start();

//sum without shipping cost
$sql= "SELECT ROUND(SUM(totalPrice), 2) as s FROM cartitems";
$result= mysqli_query($con,$sql); 
if(mysqli_num_rows($result) >0){
   $row=mysqli_fetch_assoc($result);}
$s=$row['s'];
//versandkosten
if($s < 30)
    $ship = 1;
else $ship = 2;

$sql = "SELECT cost FROM shippingcost where id= $ship";
$result = mysqli_query($con,$sql);
if(mysqli_num_rows($result) >0)
    $row=mysqli_fetch_assoc($result);
$versandkosten= $row['cost'];
//sum with shipping cost
$lastSum= $s + $versandkosten;

//count items
$sql = "SELECT sum(Quantity) as c FROM cartItems";
$result = mysqli_query($con,$sql);
if(mysqli_num_rows($result) >0)
    $row=mysqli_fetch_assoc($result);
$cnt= $row['c'];

$items = showSummary();
?>
<html>
    <head>
		<link rel="stylesheet" href="general-styling.css" />
        <link rel = "stylesheet" href="ship.css" />
        <meta charset="utf8"/>
        <title>Shipping</title>
    </head>
    <body>
		<div id="header" class="row">
			<div id="text-wrapper" class="flex-center">
				<h2>Online-Shopping Projekt</h2>
			</div>
		</div>
        
        <div id="content" class="col">
		
			<div id="container" class="row fullwidth">
        
				<div id="sidemenu" class="col one-third-box">       
					<div id="hello" class="padded-box shadow large superbold left-aligned">
						Willkommen <?php echo $_SESSION['gender'] . " " . $_SESSION['lastName'] . "!"; ?>
					</div>
				</div>
				
				<div id="shipping" class="col two-third-box padded-box large_padding">
				
					<div id="address" class="col">
						<h3>Rechnungsadresse</h3>
						<p>
							<?php 
							echo @$_SESSION['gender']." ".$_SESSION['firstName']." ".$_SESSION['lastName']."<br>"; 
							echo $_SESSION['street']." ".$_SESSION['housnr']."<br>"; 
							echo $_SESSION['plz']." ".$_SESSION['ort']."<br>";            
							?>        
							
						</p>
						<a href="register_copy.php" class="smallbutton"> ÄNDERN</a>
					</div>
					
					<div id="orderdetails" class="col fullwidth">
						<h3>Ihr Warenkorb</h3>
						<span class="smallfont">(<?php echo $cnt; ?> Artikel )</span>
						<div id="summaryTable" class="col hasmargintop">
							<div id="title" class="row border-bottom">
								<div class="smallfont smallcol center">Artikelnummer</div>
								<div class="smallfont smallcol center">Produkt Bild</div>
								<div class="smallfont largecol">Bezeichnung</div>
								<div class="smallfont smallcol center">Menge</div>
							</div>
							<?php foreach ($items as $item): ?>
							<div class="row padding-top">
								<div class="col smallfont center smallcol">
									<?php echo $item[0]; ?>
								</div>
								<div class="col smallcol"><img src="images/<?php echo $item[2]; ?>" width="80px" height="80px"></div>
								<div class="col smallfont largecol padded-box">
									<span class="font12"><?php echo $item[1]; ?></span><br>
									<?php echo $item[3]; ?>
								</div>
								<div class="col smallfont center smallcol">
									<?php echo $item[4]; ?>
								</div>
							</div>
							<?php endforeach; ?>
						</div>
							
					</div>
					<div class="col hasmargintop">
						<span class="bold">Preis:€ <?php echo $s;?></span>
						( <?php echo $cnt; ?> Artikel)<br>
						<span class="smallfont padding-top">Versandkosten:€ <?php echo $versandkosten; ?>
						(GRASTIS ab 30€)<br> </span>
						<span class="large bold colored padding-top">Summe: € <?php echo $lastSum;?></span>
										   
						<br>
						<a href="thank.php" class="bigbtn">JETZT KAUFEN</a>
					</div>
				</div>
				
			</div>
			 
			<div id="credit" class="row center hasmargintop">
				<div class="one-quarter-box center row topborder">
				<center><p class="smallprint">A php project by Ly Tran & Mi Do<br> 2AKIFT - April 2022</p></center>
			</div>
            
        </div>


	
</body>


</html>
