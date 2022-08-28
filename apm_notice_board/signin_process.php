<?php
require_once($_SERVER["DOCUMENT_ROOT"]."/top.php");
?>

<?php
    $sql = "select * from signup_info where id='$signin_user_id'";
    // echo $sql;
    $query_result = $mysqli->query($sql);

    // id가 로그인 할 때 입력한 user_id의 값과 일치하는 값이 있으면
    if($query_result->num_rows == true)
    {
        // 해당 ID의 회원이 존재할 경우 password가 일치한지 확인
        $encrypted_password = sha1($signin_user_password);
        // 컬럼명을 키로 사용하는 연관 배열 형태로 레코드를 1개씩 리턴한다.
        $row = $query_result->fetch_array(MYSQLI_ASSOC);
        
        if($row['pw'] == $encrypted_password)
        {
            // password가 일치한다.
            $_SESSION["is_signin"] = 'y';
            $_SESSION["user_id"] = $signin_user_id;
            $_SESSION['member_idx'] = $row['member_idx'];
            //print_r($_SESSION);
            
            header("Location: ".$url['root']."/signin_done.php");
            exit();
            
        }
        else
        {
            // password가 일치하지 않는다.
            $_SESSION['is_signin'] = "n";
            $_SESSION['user_id'] = '';
            $_SESSION['member_idx'] = 0;
            header("Location: ".$url['root']."/signin_done.php");
            exit();
            // echo 'Wrong password';
        }
    }
    // id가 로그인 할 때 입력한 user_id의 값과 일치하는 값이 없으면
    else
    {
        // 해당 ID의 회원이 존재하지 않을 경우
        header("Location: ".$url['root'].'signin_done.php');
        exit();
        
    }
?>
    <a href="signin.php">로그인 페이지로 돌아가기</a>
    
<?php
    $query_result->free();
    $mysqli->close();
?>