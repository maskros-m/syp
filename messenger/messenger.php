<?php

    require_once 'virtualKeyboard.php';
    $keys = ["qwertyuiop'", 'asdfghjkl,.', 'zxcvbnm'];
    $states = ['input_start', 'accepting_input', 'uppercase', 'error'];
	
    $state = 0;
    $inputVal = '';
    $msg = '';
	
    if (isset($_POST['submit'])) {
	$state = $_POST['state'];
	$inputVal = $_POST['submit'];
	$msg = $_POST['msg'];
	
	if (checkInput($state, $inputVal) == false) { $state = 0; }
		
	else if ($state == 0) {
	    if (isShift($inputVal)) {
		$state = 2;
	    } 
	    else {
		$state = 1;
		$msg = $inputVal;
	    }
	}
		
	else if ($state == 1) {
	    if (isAlpha($inputVal)) { $msg = $msg . $inputVal; }

	    else if (isSpace($inputVal)) { $msg .= ' ';	}

	    else if (isShift($inputVal)) { $state = 2;	}

	    else if (isBackSpc($inputVal)) {
		$msgToString = substr($msg, 0, -1);
		$msg = $msgToString;

		if (strlen($msg) == 0) { $state = 0; }
	    }
	    else if (isDel($inputVal)) {
		$state = 0;
		$msg = '';
	    }
	}
		
	else if ($state == 2) {
	    if (isAlpha($inputVal)) { $msg .= chr(ord($inputVal) - 32);
		// $state = 1; // return to lowercase. Disable this if CAPSLOCK function is more desirable.
	    }
	    else if (isSpace($inputVal) && strlen($msg) != 0) { $msg .= ' ';
	    	// same as above
	    }
	    else if (isSpace($inputVal) && strlen($msg) == 0) { $state = 0; } //WhatsApp's minor inconsistency
	    else if (isShift($inputVal)) { $state = 1;
	    	// disable this whole branch if CAPSLOCK is not intended.
	    }
	    else if (isBackSpc($inputVal)) {
	    	$msgToString = substr($msg, 0, -1);
	    	$msg = $msgToString;

	    	if (strlen($msg) == 0) { $state = 0; }
	    }
	    else if (isDel($inputVal)) {
	    	$state = 0;
	    	$msg = '';
	    }
	}		
    }
?>
<html>
    <head>
	<title>My Messenger</title>
	<link rel="stylesheet" type="text/css" href="./messenger-styling.css" />

    </head>
	
    <body>
	<div id="wrapper">
	    <div id="top">
		<h2>"Because Rome wasn't built in a day.<br>
			And WhatsApp must have started somewhere too."
		</h2>
	    </div>

	    <div id="container"> </div>

	    <div id="namepending">
		<form action="messenger.php" method="POST">
		    <input type="hidden" name="state" value="<?php echo $state ?>" />
		    <input type="hidden" name="input" value="<?php echo $inputVal ?>" />
		    <input type="hidden" name="msg" value="<?php echo $msg; ?>" />

		    <div id="display">
		    	<input readonly id="textfield" type="text" class="screen" placeholder="Type a message" value="<?php echo $msg ?>" />
		    </div>

		    <div id="keyboard">
			<?php foreach ($keys as $keyrow): ?>
			    <?php for ($i = 0; $i < strlen($keyrow); $i++): ?>
				<input type="submit" class="key" name="submit" value="<?php echo $keyrow[$i]?>" />
			    <?php endfor; ?>
			 <?php endforeach; ?>

			 <input id="del" type="submit" name="submit" class="key" value="del" />
			 <input type="submit" name="submit" class="key" value="Shift" />
			 <input type="submit" name="submit" class="backspc" value="BackSpc" />
			 <input type="submit" name="submit" class="space" value="SPACE" />
		    </div>

		</form>
	    </div>
	</div>
    </body>
</html>
