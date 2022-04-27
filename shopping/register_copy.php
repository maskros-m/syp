       

<?php
session_start();
$firstName="";
$lastName ="";
$phone ="";
$email ="";
$street="";
$housenr="";
$plz="";
$ort="";


require_once 'dbTools.php';
require_once 'functions.php';


if(isset($_REQUEST['submit'])){  
    $gender = $_POST['gender'];  
    $firstName =$_POST['firstName'];
    $lastName =$_POST['lastName'];
    $birthDate =$_POST['birthDate'];
    $phone =$_POST['phone'];
    $email =$_POST['email'];
    $street=$_POST['street'];
    $housenr=$_POST['housenr'];
    $plz=$_POST['plz'];
    $ort=$_POST['ort'];
    $pwd=md5($_POST['pwd']);
    $pwd2=md5($_POST['pwd2']);
      

    validateAccount($firstName, $lastName,$birthDate,$email,$phone,$street,$housenr,$plz,$ort,$pwd,$pwd2);
    if(count($errorMessages)== 0){ 
       
        updateOrInsertAccount($firstName,$lastName,$birthDate,$email,$phone,$street,$housenr,$plz,$ort,$pwd,$gender);
        $sql = "SELECT * FROM customers WHERE email = '$email'";
        $result = mysqli_query($con,$sql);
        if(mysqli_num_rows($result) >0){
            $row=mysqli_fetch_assoc($result);
            $_SESSION['id']=$row['id'];
            $_SESSION['gender']=$row['gender'];
            $_SESSION['firstName']= $row['firstName'];
            $_SESSION['lastName']= $row['lastName'];
            $_SESSION['birthDate']= $row['birthDate'];
            $_SESSION['email']= $row['email'];
            $_SESSION['phone']= $row['phone'];
            $_SESSION['street']= $row['street'];
            $_SESSION['housnr']= $row['housnr'];
            $_SESSION['plz']= $row['plz'];
            $_SESSION['ort']= $row['ort'];
            header("Location: ship.php");
        }      

    }
}
?>
<html>
    <head>
		<link rel="stylesheet" href="general-styling.css" />
        <link rel = "stylesheet" href="login.css" />
        <meta charset="utf8"/>
        <title>Kundendetails</title>
    </head>
    <body>
        
        <div id="wrapper" class="row">
			<div id="sideimg"><img src="images/blueflowers.png"></div>
		
			<div id="loginwrapper">
				<div id="container" class="col">
				
					<div id="login">
						<span class="border-black block behind-text text-centre mt-horizontal-line"><span><h1>Kundendetails ändern</h1></span></span>
					</div>
					
					<div id="errorMessages" class="fullwidth">
						<?php foreach ($errorMessages as $message): ?>
						<div class="redbg flex fullwidth padding text-centre small_font red bold"><?php echo $message?></div>
						<?php endforeach; ?>
					</div>
            
					<form id="loginForm" action="register.php" method="POST">
					<div id="logintable">
						Titel
						<input type="radio" name="gender" value="Frau" ><label>Frau</label>
						<input type="radio" name="gender" value="Herr" checked="checked"><label>Herr</label> 
                            
						<div id="label-fname" class="formwrapper relative">
							<label class="inline-block relative bottom-margin min-height subscript">Vorname *</label>
							<div class="flex"><input type="text" name="firstName" placeholder="Ihr Vorname" value="<?php echo $_SESSION['firstName']; ?>" required /></div>
						</div>	
							
							
                        <div id="label-lname" class="formwrapper relative">
							<label class="inline-block relative bottom-margin min-height subscript">Nachname *</label>
							<div class="flex"><input type="text" name="lastName" placeholder="Ihr Nachname" value="<?php echo $_SESSION['lastName']; ?>" required /></div>
						</div>
						
						<div id="label-dob" class="formwrapper relative">
							<label class="inline-block relative bottom-margin min-height subscript">Geburtsdatum *</label>
							<div class="flex"><input type="date" name="birthDate" placeholder="" value="<?php echo $_SESSION['birthDate']; ?>" required /></div>
						</div>

						<div id="label-email" class="formwrapper relative">
							<label class="inline-block relative bottom-margin min-height subscript">Email-Adresse *</label>
							<div class="flex"><input type="text" name="email" placeholder="Email-Adresse" value="<?php echo $_SESSION['email']; ?>" required /></div>
						</div>
						
                        <div id="label-phone" class="formwrapper relative">
							<label class="inline-block relative bottom-margin min-height subscript">Telefon</label>
							<div class="flex"><input type="text" name="phone" placeholder="Telefonnummer" value="<?php echo $_SESSION['phone']; ?>" /></div>
						</div>
						
                        <div id="label-country" class="row small_font">
							Land: Österreich
						</div>
						
						<div id="label-str" class="formwrapper relative">
							<label class="inline-block relative bottom-margin min-height subscript">Straße *</label>
							<div class="flex"><input type="text" name="street" placeholder="Straße" value="<?php echo $_SESSION['street']; ?>" required /></div>
						</div>
						
                        <div class="row flex-space">
							<div id="label-house" class="formwrapper relative halfwidth">
								<label class="inline-block relative bottom-margin min-height subscript">Hausnummer *</label>
								<div class="flex fullwidth"><input type="text" name="housenr" placeholder="Hausnummer" value="<?php echo $_SESSION['housnr']; ?>" required /></div>
							</div>
									
							<div id="label-postcode" class="formwrapper relative halfwidth">
								<label class="inline-block relative bottom-margin min-height subscript">Plz *</label>
								<div class="flex fullwidth"><input type="text" name="plz" placeholder="8010" value="<?php echo $_SESSION['plz']; ?>" required /></div>
							</div>
						</div>
						
                        <div id="label-ort" class="formwrapper relative">
							<label class="inline-block relative bottom-margin min-height subscript">Ort *</label>
							<div class="flex"><input type="text" name="ort" placeholder="Graz" value="<?php echo $_SESSION['ort']; ?>" required /></div>
						</div>
                        
                        <div id="label-pwd" class="formwrapper relative">
							<label class="inline-block relative bottom-margin min-height subscript">Neues Passwort *</label>
							<div class="flex"><input type="password" name="pwd" placeholder="" required /></div>
						</div>

						<div id="label-pwd" class="formwrapper relative">
							<label class="inline-block relative bottom-margin min-height subscript">Neues Passwort bestätigen *</label>
							<div class="flex"><input type="password" name="pwd2" placeholder="" required /></div>
						</div>
                                           
                        <input  type="submit" name="submit" value="ÄNDERUNGEN BESTÄTIGEN"/>
					</div>
					</form>
					
					<div id="bottom"><p></p><span class="smallprint">A project by Ly Tran & Mi Do (2AKIFT)</span></div>
				</div>
			</div>
			
			<div id="rightborder"></div>
        </div>

    </body>
</html>