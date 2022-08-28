<?php
    require_once($_SERVER['DOCUMENT_ROOT']."/top.php");
    include_once($_SERVER['DOCUMENT_ROOT']."/header.php");
?>

<?php
    $is_signin = $_SESSION['is_signin'];

    if($is_signin == 'y')
    {
        $user_id = $_SESSION['user_id'];
        $message = $user_id.'님, 로그인 되었습니다.';
        header("Location: ".$url['root']."index.php");
        exit();
    }
    else
    {
        $_SESSION['is_signin'] = "n";
        $_SESSION['user_id'] = 'Unknown';
        echo "로그인이 실패되었습니다.<br>";
        //header("Location: ".$url['root']."signin.php");
        //exit();
    }
?>

<?php  
    include_once($_SERVER['DOCUMENT_ROOT']."/footer.php");
?>