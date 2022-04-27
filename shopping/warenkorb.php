<?php
	require_once 'dbAccess.php';
	
	$articles = showCart();
	$totalart = getSumCol("Quantity", "cartitems");
		
	$plusId = @$_GET['plusId'];
	$minusId = @$_GET['minusId'];
	$sum = getSum("totalPrice", "cartitems");
	if ($plusId != '') {
		increaseQty($plusId);
		header("Refresh:0; url=warenkorb.php");
		$sum = getSum("totalPrice", "cartitems");
	}
	if ($minusId != '') {
		if (getQty($minusId) <= 1) {
			removeItem($minusId);
			header("Refresh:0; url=warenkorb.php");
			$sum = getSum("totalPrice", "cartitems");
		} else {
			decreaseQty($minusId);
			header("Refresh:0; url=warenkorb.php");
			$sum = getSum("totalPrice", "cartitems");
		}
	}
	
	if ($sum[0][0] >= 30) {
		$shippingcost = 0.00;
	} else {$shippingcost = 3.99;}
	
	$total = $sum[0][0] + $shippingcost;
?>

<html>

<head>
	<title>Warenkorb</title>
	<link rel="stylesheet" href="general-styling.css" />
	<link rel="stylesheet" href="cart.css" />
</head>

<body>
	<div id="header">
		<div id="text-wrapper" class="flex-center">
			<h2>Ihr Warenkorb</h2>
		</div>
	</div>
	
	<div id="content" class="row left-aligned">
		<div id="cartcontent" class="col two-third-box padded-box padding">
			<div id="yourcart" class="bigletter padded-box">Ihr Warenkorb</div>
			<div id="itemlist" class="col padded-box small_padding">
			<?php foreach ($articles as $article): ?>
				
				<div class="article row shadow">
					<img src="images/<?php echo $article[2]?>" width="150px" height="150px">
					<div id="name-desc" class="col grow-2 padded-box">
						<div><b><?php echo $article[0]?></b></div>
						<div class="smallfont"><p><?php echo $article[1]?></p></div>
						<div class="smallfont"><?php echo $article[6]?> <?php echo $article[7]?></div>
					</div>
					
					<div id="price-qty" class="col padded-box one-quarter-box">
						<div id="qty" class="row smallfont grow-1 padded-box right-aligned">
							<div><a class="smallfont smallsquare" href="warenkorb.php?minusId=<?php echo $article[5]?>">-</a></div>
							<div><input readonly type="text" class="unit" name="quantity" value="<?php echo $article[3] ?>"></div>
							<div><a class="smallfont smallsquare" href="warenkorb.php?plusId=<?php echo $article[5]?>">+</a></div>
						</div>
						<div id="price" class="grow-1 padded-box right-aligned price">€<?php echo $article[4]?></div>
					</div>
				</div>
			<?php endforeach;?>
			</div>
			

		</div>
		
		<aside class="col one-third-box padded-box">
			<div id="sum" class="col sticky ontop top_100 padding">
				<div class="row hasmargintop">
					<div class="grow-1 left-aligned"><span class="bold normal">Preis</span><span class="small"> (<?php echo $totalart[0][0]?> Artikel)</span></div>
					<div class="grow-1 right-aligned"><span class="bold normal">€ <?php echo $sum[0][0]?></span></div>
				</div>
				<div class="row hasmargintop">
					<div class="col fullwidth">
						<div class="row">
							<div class="grow-1 left-aligned"><span class="bold small">Versandkosten</span></div>
							<div class="grow-1 right-aligned"><span class="bold small">€ <?php echo $shippingcost?></span></div>
						</div>
						
						<div class="row left-aligned"><span class="small">(GRATIS ab 30€)</span></div>
					</div>
				</div>
				<div class="row hasmargintop dottedborder">
					<div class="grow-1 left-aligned"><span class="superbold large">Summe</span></div>
					<div class="grow-1 right-aligned"><span class="superbold large colored">€ <?php echo $total?></span></div>
				</div>
		
				<div id="bigbutton"><a class="bigbtn flex-end" href="./login.php">ZUR KASSA</a></div> 
				
				<div id="bigbutton"><a class="bigbtn flex-end" href="./einkauf.php">ZURÜCK ZUM EINKAUFEN</a></div> 
			</div>
		</aside>
		
			
		</div>
	</div>
	
	<div id="credit" class="row center">
		<div class="one-quarter-box center row topborder">
			<center><p class="smallprint">A php project by Ly Tran & Mi Do<br> 2AKIFT - April 2022</p></center>
		</div>
	</div>
	
</body>


</html>