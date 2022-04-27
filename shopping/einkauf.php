<?php
	require_once './dbAccess.php';
	
	if (isset($_POST['all'])) {
		$items = getItems("products");
	} elseif (isset($_POST['gesicht'])) {
		$items = sortByCat(1);
	} elseif (isset($_POST['makeup'])) {
		$items = sortByCat(2);
	} elseif (isset($_POST['parfum'])) {
		$items = sortBycat(3);
	} elseif (isset($_POST['korper'])) {
		$items = sortByCat(4);
	} elseif (isset($_POST['haare'])) {
		$items = sortByCat(5);
	} else {
		$items = getItems("products");
	}
	
	
	$productId = @$_GET['prodId'];
	if ($productId != '') {
		if (itemExists($productId, "cartitems")) {
			$id = getId($productId);
			increaseQty($id);
		} else {
			addToCart($productId);			
		}
	}
	
	$arts = showCart();
	
?>

<html>

<head>
	<title>Lieferung</title>
	<link rel="stylesheet" href="general-styling.css" />
	<link rel="stylesheet" href="einkauf.css" />
	<meta charset="utf8"/>
</head>

<body>
	<div id="header" class="row">
		<div id="text-wrapper" class="flex-center">
			<h2>Online-Shopping Projekt</h2>
		</div>
	</div>
	
	<div id="menu" class="row">
		<div id="category" class="row three-quarter-box middle-aligned">
		<form action="einkauf.php" method="POST">
			<span class="decor"><input type="submit" name="all" value="ALLE"></span>
			<span class="decor"><input type="submit" name="gesicht" value="GESICHT"></span>
			<span class="decor"><input type="submit" name="makeup" value="MAKE-UP"></span>
			<span class="decor"><input type="submit" name="parfum" value="PARFUM"></span>
			<span class="decor"><input type="submit" name="korper" value="KÖRPER"></span>
			<span class="decor"><input type="submit" name="haare" value="HAARE"></span>
		</form>
		</div>
	</div>
	
	<div id="content" class="row">
		<div id="products" class="row three-quarter-box flex-space">
			<div id="product-container" class="row wrapped">
			<?php foreach ($items as $item): ?>
			
				<div class="item">
					<img src="images/<?php echo $item[6]?>">
					<div class="itemname"><?php echo $item[1]?></div>
					<div class="smallfont"><?php echo $item[2]?> <?php echo $item[3]?></div>
					<div class="price">€<?php echo $item[4]?></div>
					<form action="einkauf.php" method="POST">
						<a class="smallbtn" href="einkauf.php?prodId=<?php echo $item[0]?>">In den Warenkorb</a>
					</form>
				</div>
					
			<?php endforeach; ?>
			</div>
		</div>
		
		<div id="cartpreview" class="col one-quarter-box">
			<div id="cartitems" class="col">
				<div id="cartinfo" class="row center"><div id="header-wrapper" class="horizontal-line row center"><h3>Ihr Warenkorb</h3></div></div>
				<?php foreach ($arts as $art):?>
					<div id="cartwrapper" class="row flex-space flex-center">
						<div class="left-padding"></div>
						<div class="one-quarter-box flex-center"><img src="images/<?php echo $art[2]?>" width="50px" height="50px"></div>
						<div class="three-quarter-box flex-center">
						<div class="smallfont flex-center two-third-box"><?php echo $art[0]?></div>
						<div class="smallfont flex-center one-quarter-box right-aligned"><?php echo $art[3] ?></div>
						</div>
					</div>
				<?php endforeach; ?>
			</div>
			<div id="button"><a class="bigbtn flex-end" href="./warenkorb.php">ZUM WARENKORB</a></div>
		</div>
	</div>
	
	<div id="credit" class="row center">
		<div class="one-quarter-box center row topborder">
			<center><p class="smallprint">A php project by Ly Tran & Mi Do<br> 2AKIFT - April 2022</p></center>
		</div>
	</div>
	
</body>


</html>