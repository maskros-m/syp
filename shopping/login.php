
<?php
session_start();
$msg="";


require_once 'dbTools.php';
require_once 'functions.php';


if(isset($_REQUEST['submit'])){    
    
    $email =$_POST['email'];
    $password=md5($_POST['pwd1']);
    $sql = "SELECT * FROM customers WHERE email = '$email' AND password = '$password'";
    $result = mysqli_query($con,$sql);
    if(mysqli_num_rows($result) >0){
        $row=mysqli_fetch_assoc($result);
		
		$_SESSION['gender']=$row['gender'];
		$_SESSION['firstName']= $row['firstName'];
		$_SESSION['lastName']= $row['lastName'];
		$_SESSION['birthDate']= $row['birthDate'];
		$_SESSION['email']= $row['email'];
		$_SESSION['phone']= $row['phone'];
		$_SESSION['street']= $row['street'];
		$_SESSION['housnr']= $row['housNr'];
		$_SESSION['plz']= $row['plz'];
		$_SESSION['ort']= $row['ort'];
		$_SESSION['id']=$row['id'];
		/*
		$_SESSION['gender']=$row['gender'];
        $_SESSION['firstName']= $row['firstName'];
        $_SESSION['lastName']= $row['lastName'];
        $_SESSION['street']= $row['street'];
        $_SESSION['housnr']= $row['housnr'];
        $_SESSION['plz']= $row['plz'];
        $_SESSION['ort']= $row['ort'];
		*/
        header("Location: ship.php");
    }
    else{
        $msg = "Email oder Passwort ist nicht richtig!";
    }    
}
?>
<html>
    <head>
		<link rel="stylesheet" href="general-styling.css" />
        <link rel="stylesheet" href="login.css" />
		
        <meta charset="utf8"/>
        <title>Login</title>
        
    </head>
    <body>
       
        <div id="wrapper" class="row">
			
			<div id="sideimg"><img src="images/flowerbg.png"></div>
            
            <div id="loginwrapper">
				
				<div id="container" class="col">
					<div id="login">
						<span class="border-black block behind-text text-centre mt-horizontal-line"><span><h1>Einloggen</h1></span></span>
					</div>
					
					<form id="loginForm" action="login.php" method="POST">                
					<div id="logintable">  

							<div id="error" class="small_font red"><?php echo $msg; ?></div>	
						
							<div id="label-email" class="formwrapper relative">
								<label class="inline-block relative bottom-margin min-height subscript">Email-Adresse</label>
								<div class="flex"><input type="text" name="email" placeholder="Email-Adresse" required /></div>
							</div>
									 
							<div id="label-password" class="formwrapper relative"> 
								<label class="inline-block relative bottom-margin min-height subscript">Passwort</label>
								<div class="flex"><input type="password" name="pwd1" placeholder="Passwort" required /></div>
							</div>
														   
							<input  type="submit" name="submit" value="EINLOGGEN"/>

						
					</div>
					</form>
					
					<div id="konto">
						<span class="border-black block behind-text text-centre mt-horizontal-line"><span><h1>Sie haben kein Konto?</h1></span></span>
						<p></p>
						<a href="register.php" class="bigbtn">KONTO ERSTELLEN</a>
					</div>
					

					
					<div id="bottom"><p></p><span class="smallprint">A project by Ly Tran & Mi Do (2AKIFT)</span></div>
				</div>

            </div>
			<div id="rightborder"></div>
			
			
        </div>
    </body>
</html>