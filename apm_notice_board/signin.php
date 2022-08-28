<?php
    require_once($_SERVER["DOCUMENT_ROOT"]."/top.php");
    include_once($_SERVER['DOCUMENT_ROOT']."/header.php");
?>
        <form name="signin_form" action="./signin_process.php" method="post">
            <p>User ID : <input type="text" name="signin_user_id" placeholder="User ID" autofocus required></input></p>
            <p>Password : <input type="password" name="signin_user_password" placeholder="User Password" autofocus required></input></p>
            <p><input type="submit" value="로그인"></input>
        </form>
<?php
    include $_SERVER['DOCUMENT_ROOT'].'/footer.php';
?>