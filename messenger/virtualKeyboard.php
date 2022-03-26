<?php

	function isAlpha($c) {
		return strpos("abcdefghijklmnopqrstuvwxyz',.", $c) === false ? false : true;
	}
	
	function isSpace($c) {
		return $c == 'SPACE' ? true : false;
	}
	
	function isShift($c) {
		return $c == 'Shift' ? true : false;
	}
	
	function isBackSpc($c) {
		return $c == 'BackSpc' ? true : false;
	}
	
	function isDel($c) {
		return $c == 'del' ? true : false;
	}
	
	function send($c) {
		return $c == 'Send' ? true : false;
	}
	
	function checkInput($programme_state, $c) {
		$states = ['input_start', 'accepting_input', 'finished', 'error'];
		
		if (($programme_state == 0 && (isAlpha($c) || isShift($c)))
			|| ($programme_state == 1 && (isAlpha($c) || isSpace($c) || isShift($c) || isBackSpc($c) || isDel($c)) || send($c))
			|| ($programme_state == 2 && (isAlpha($c) || isSpace($c) || isShift($c) || isBackSpc($c) || isDel($c)) || send($c))
			// || ($programme_state == 3 && (isAlpha($c) || isShift($c)))
		) return true;
		return false;
	}
?>
