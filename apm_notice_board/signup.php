<?php
    require_once($_SERVER['DOCUMENT_ROOT']."/top.php");
    include_once($_SERVER['DOCUMENT_ROOT']."/header.php");
?>
        <form name="signup_form" action="./signup_process.php" method="post">
            <p>User Name : <input type="text" name="signup_username" placeholder="User Name" autofocus required></input></p>
            <p>User ID : <input type="text" name="signup_user_id" placeholder="User ID" autofocus required></input></p>
            <p>Password : <input type="password" name="signup_user_password" placeholder="User Password" autofocus required></input></p>
            <p>Password Check : <input type="password" name="signup_user_password_check" placeholder="User Password Check" autofocus required></input></p>
            <p><input type="submit" value="회원가입"></input>
        </form>
<?php
    include $_SERVER['DOCUMENT_ROOT'].'/footer.php';
?>