<?php
	$host = "localhost";
	$dbUser = "root";
	$dbPwd = "";
	$database = "syp_shopping";
	

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
		$sql = "INSERT INTO cartitems (ProductId, Quantity, Price, totalPrice) VALUES ('$productId', 1, $price, $price * 1)";
		mysqli_query($connection, $sql);
		
		/*
		$sql2 = "SELECT * from cartitems";
		$result = mysqli_query($connection, $sql2);
		$res = mysqli_fetch_all($result);
		print_r($res);
		*/ 
	}
	
	function increaseQty($id) {
		global $connection;
		$sql = "UPDATE cartitems SET Quantity = (Quantity + 1), totalPrice = round(Price * Quantity, 2) WHERE id = $id";
		mysqli_query($connection, $sql);	
	}
	
	function decreaseQty($id) {
		global $connection;
		$sql = "UPDATE cartitems SET Quantity = (Quantity - 1), totalPrice = round(Price * Quantity, 2) WHERE id = $id";
		mysqli_query($connection, $sql);	
	}
	
	function removeItem($id) {
		global $connection;
		$sql = "DELETE FROM cartitems WHERE id = $id";
		mysqli_query($connection, $sql);
	}
	
	function showCart() {
		global $connection;
		$sql = "SELECT p.PrdName, p.PrdDesc, i.Quantity, i.totalPrice, i.id FROM products p, cartitems i WHERE i.ProductId = p.id ORDER BY i.id";
		$result = mysqli_query($connection, $sql);
		return mysqli_fetch_all($result);
	}
	
	function getSum($column, $table) {
		global $connection;
		$sql = "SELECT ROUND(SUM($column), 2) FROM $table";
		$result = mysqli_query($connection, $sql);
		return mysqli_fetch_all($result);
	}
?>