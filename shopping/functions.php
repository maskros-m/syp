<?php
$errorMessages = [];
function validateAccount($firstname, $lastname,$date,$mail,$phone,$street,$housenr,$plz,$ort,$pwd,$pwd2){
    global $errorMessages;
    $errorMessages = [];
    if(!validateAlphabet($firstname))
        $errorMessages[] = "ungültiger Vorname";
    if(!validateAlphabet($lastname))
        $errorMessages[] = "ungültiger Nachname";
    if(!validateAlphabet($street))
        $errorMessages[] = "ungültige Strasse"; 
    if(!validateAlphabet($ort))
        $errorMessages[] = "ungültiger Ort";
    
    if(!validateEmail($mail))
        $errorMessages[] = "ungültige Email";
    if($phone !=""){
        if(!(is_numeric($phone)))
        $errorMessages[] = "ungültige Telefonnummer";
    }
    
    if(!is_numeric($plz))
        $errorMessages[] = "ungültige Postleitzahl";
    if(!is_numeric($housenr))
        $errorMessages[] = "ungültige Hausnummer";
    if($pwd != $pwd2)
        $errorMessages[]= "die Passworte sind nicht gleich!";
    if((strtotime($date) > strtotime("now")) || !validateDate($date))
        $errorMessages[]= "ungültiges Geburtsdatum";
//  echo "tele:".$phone."etwas";
}
function validateAlphabet($name){
    return preg_match("/[A-Za-z\öäüß ]{1,30}$/",$name);
}
function validateEmail($mail){
    return preg_match("/[A-Za-z\_.@* -]{5,30}$/",$mail);
}
function validateDate($date){
    return preg_match("/^\d{4}\-\d{2}\-\d{2}$/",$date);
}

?>

