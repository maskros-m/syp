<?php

$dbUser ="root";
$dbPwd="";
$dbHost = "localhost";
$database = "syp_shopping_1"; 

$con =@mysqli_connect($dbHost,$dbUser,$dbPwd,$database);

if(!$con)
    die("error connecting to mysql db $database");

function accountExist($email){
    global $con;
    $sql = "SELECT id FROM customers WHERE email = '$email'";
    $result = mysqli_query($con,$sql);
    return mysqli_num_rows($result) > 0? True : False;
}
function updateOrInsertAccount($firstName,$lastName,$birthDate,$email,$phone,$street,$housenr,$plz,$ort,$pwd,$gender){
    global $con;
    if(accountExist($email)){
        $sql = "UPDATE customers SET firstName = '$firstName',lastName = '$lastName', birthdate = '$birthDate',email='$email',phone='$phone', street ='$street',housnr='$housenr',plz='$plz',ort ='$ort',password='$pwd',gender='$gender' WHERE email = '$email' AND password ='$pwd' ";        
    }
    else{
        $sql ="INSERT INTO customers(firstName,lastName,birthdate,email,phone,street,housnr,plz,ort,password,gender) VALUES('$firstName','$lastName','$birthDate','$email','$phone','$street','$housenr','$plz','$ort','$pwd','$gender')";      

    }
    mysqli_query($con,$sql);
}
function getOrderId($customerId){
    global $con;
    $sql ="SELECT id as i FROM orders where customerId = $customerId";
    $result= mysqli_query($con,$sql);
    if(mysqli_num_rows($result) >0){
        $row=mysqli_fetch_assoc($result);}
    return $row['i'];
}
// function insertOrders($col,$value){
//     global $con;
//     $sql= "INSERT INTO orders($col) VALUES('$value')";
//     mysqli_query($con,$sql);
// }
    
   
    
    
 
        



    

    








?>

