<?php
	
	require_once 'calTools.php';
	
	$btnSymbols = ["123+", "456-", "789*", "0=C/"];
	$states = ["op1_start", "op1", "op2_start", "op2", "finished", "error"];
	$stateInx = 0;
	$result = 0;
	$op1 = 0;
	$operator = "undefined";
	
	if (isset($_POST['submit'])) {
		$stateInx = $_POST['stateInx'];
		$input = $_POST['submit'];
		$result = $_POST['result'];
		$op1 = $_POST['op1'];
		$operator = $_POST['operator'];
		/* echo "checkState: " . checkState($stateInx, $op) == True ? "okay" : "error"; */
		
		if (checkState($stateInx, $input) == false) {
			$stateInx = 5;
			$result = "Error";
		} 
		else if ($stateInx == 0) {
			$stateInx = 1; 
			$result = $input; // starts taking first digit
		} 
		else if ($stateInx == 1) {
			if (isDigit($input)) $result = $result * 10 + $input;
			else { // if input is no longer digit
				$stateInx = 2;
				$op1 = $result; // assign digit string to left operand
				$operator = $input; // save input arithmetic operator to variable
			}
		} 
		else if ($stateInx == 2) {
				$stateInx = 3;				
				$result = $input;
		} 
		else if ($stateInx == 3) {
			if (isDigit($input)) $result = $result * 10 + $input;
			else if ($input == '=') { 
				$stateInx = 4; 
				$result = calculate($operator, $op1, $result);
			}
		}
		else if ($stateInx == 4 || $stateInx == 5) {
			if ($input == 'C') { 
				$result = 0;
				$stateInx = 0;
			}
		}
	}
?>


<html>
	<head>
		<title>Calculator Rebuilt</title>
		<link rel="stylesheet" type="text/css" href="calculator_styling.css" />
	</head>
	
	<body>
		<div id="top">
			<h2>Einfacher Taschenrechner</h2>
		</div>
		
		<div id="container">
			<form action="calc_rebuild.php" method="POST">
				<input type="hidden" name="stateInx" value="<?php echo $stateInx ?>" />
				<input type="hidden" name="result" value="<?php echo $result ?>" />
				<input type="hidden" name="op1" value="<?php echo $op1 ?>" />
				<input type="hidden" name="operator" value="<?php echo $operator ?>" />
				
				<div id="calculator">
				<input type="text" class="fullWidth" value="<?php echo $result ?>" />
				<?php foreach ($btnSymbols as $btnRow): ?>
					<?php for($i = 0; $i < strlen($btnRow); $i++): ?>
						<input class="item" type="submit" name="submit" value="<?php echo $btnRow[$i] ?>" />
					<?php endfor; ?>
				<?php endforeach; ?>
				</div>
			</form>
		</div>
	</body>
</html>