<?php
	$host = "localhost";
	$dbUser = "root";
	$dbPwd = "";
	$database = "syp_shopping_1";
	

	$connection = @mysqli_connect($host, $dbUser, $dbPwd, $database);
	if (!$connection)
		die("Error connecting to database");
	
	function getItems($table) {
		global $connection;
		$selectItems = "SELECT * FROM $table";
		$result = mysqli_query($connection, $selectItems);
		return mysqli_fetch_all($result);
	}
	
	function itemExists($id, $table) {
		global $connection;
		$sql = "SELECT id FROM $table WHERE ProductId = $id";
		$result = mysqli_query($connection, $sql);
		return mysqli_num_rows($result) > 0 ? true : false;
	}
	
	function getPrice($productId) {
		global $connection;
		$sql = "SELECT price FROM products WHERE id = '$productId'";
		$result = mysqli_query($connection, $sql);
		$price = mysqli_fetch_all($result);
		return $price[0][0];
	}
	
	function getProductId($id) {
		global $connection;
		$sql = "SELECT ProductId FROM cartitems WHERE id = $id";
		$result = mysqli_query($connection, $sql);
		$Productid = mysqli_fetch_all($result);
		return $Productid[0][0];
	}
	
	function getId($productId) {
		global $connection;
		$sql = "SELECT id FROM cartitems WHERE ProductId = $productId";
		$result = mysqli_query($connection, $sql);
		$id = mysqli_fetch_all($result);
		return $id[0][0];
	}
	
	function getQty($productId) {
		global $connection;
		$sql = "SELECT Quantity FROM cartitems WHERE id = '$productId'";
		$result = mysqli_query($connection, $sql);
		$qty = mysqli_fetch_all($result);
		return $qty[0][0];
	}
	
	function addToCart($productId) {
		global $connection;
		$price = getPrice($productId);
		$sql = "INSERT INTO cartitems (ProductId, Quantity, totalPrice) VALUES ('$productId', 1, $price * 1)";
		mysqli_query($connection, $sql);
		
	}
	
	function increaseQty($id) {
		global $connection;
		$productId = getProductId($id);
		$price = getPrice($productId);
		$sql = "UPDATE cartitems SET Quantity = (Quantity + 1), totalPrice = round($price * Quantity, 2) WHERE id = $id";
		mysqli_query($connection, $sql);	
	}
	
	function decreaseQty($id) {
		global $connection;
		$productId = getProductId($id);
		$price = getPrice($productId);
		$sql = "UPDATE cartitems SET Quantity = (Quantity - 1), totalPrice = round($price * Quantity, 2) WHERE id = $id";
		mysqli_query($connection, $sql);	
	}
	
	function removeItem($id) {
		global $connection;
		$sql = "DELETE FROM cartitems WHERE id = $id";
		mysqli_query($connection, $sql);
	}
	
	function showCart() {
		global $connection;
		$sql = "SELECT p.PrdName, p.PrdDesc, p.Img, i.Quantity, i.totalPrice, i.id, p.Quantity, p.Unit FROM products p, cartitems i WHERE i.ProductId = p.id ORDER BY i.id";
		$result = mysqli_query($connection, $sql);
		return mysqli_fetch_all($result);
	}
	
	function getSum($column, $table) {
		global $connection;
		$sql = "SELECT ROUND(SUM($column), 2) FROM $table";
		$result = mysqli_query($connection, $sql);
		return mysqli_fetch_all($result);
	}
	
	function sortByCat($categoryNo) {
		global $connection;
		$sql = "SELECT * from products WHERE catId = $categoryNo";
		$result = mysqli_query($connection, $sql);
		return mysqli_fetch_all($result);
	}
	
	function getSumCol ($column, $table) {
		global $connection;
		$sql = "SELECT SUM($column) FROM $table";
		$result = mysqli_query($connection, $sql);
		if (mysqli_num_rows($result) >0) {
			return mysqli_fetch_all($result);
		}
		else {
			return array(array(0));
		}
	}
	
	function showSummary() {
		global $connection;
		$sql = "SELECT c.ProductId, p.PrdName, p.Img, p.PrdDesc, c.Quantity FROM cartitems c, products p WHERE c.ProductId = p.id";
		$result = mysqli_query($connection, $sql);
		return mysqli_fetch_all($result);
	}
?>