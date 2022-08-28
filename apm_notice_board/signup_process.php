<?php
    require_once($_SERVER['DOCUMENT_ROOT']."/top.php");
?>

<?php
    //echo $signup_username. '<br>';
    //echo $signup_user_id. '<br>';
    //echo $signup_user_password. '<br>';
    //echo $signup_user_password_check. '<br>';

    $encrypted_password = sha1($signup_user_password);
    $encrypted_password_check = sha1($signup_user_password_check);

    $sql = "insert into signup_info (username, id, pw, pw_ck) values ('$signup_username', '$signup_user_id', '$encrypted_password', '$encrypted_password_check')";

    $query_result = $mysqli->query($sql);

    $mysqli->close();

    header("Location: ".$url['root']."/signup_done.php");
    exit();
?>