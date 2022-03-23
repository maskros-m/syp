<?php 

	enum States {
		
	}
	function isDigit($c) {
		return strpos('1234567890', $c) === false ? false : true;
	}
	
	function isArithOp($c) {
		return strpos('+-*/', $c) >= 0 ? true : false;
	}
	
	function checkState($stateInx, $op) {
		// isDigit($op);
		$states = ["op1_start", "op1", "op2_start", "op2", "finished", "error"];
		
		if (($stateInx == 0 && isDigit($op))
			|| ($stateInx == 1 && (isArithOp($op) || isDigit($op)))
			|| ($stateInx == 2 && isDigit($op))
			|| ($stateInx == 3 && (isDigit($op) || $op == "="))
			|| ($stateInx == 4 && $op == 'C')
			|| ($stateInx == 5 && $op == 'C')
			) {return true;}
		return false;
	}
	
	function calculate($operator, $left_operand, $right_operand) {
		switch ($operator) {
			case '+': return $left_operand + $right_operand;
			case '-': return $left_operand - $right_operand;
			case '*': return $left_operand * $right_operand;
			case '/': return $left_operand / $right_operand;
		}
	}

?>