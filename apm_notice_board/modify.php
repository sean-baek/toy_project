<?php
    require_once($_SERVER['DOCUMENT_ROOT']."/top.php");
    include_once($_SERVER['DOCUMENT_ROOT']."/header.php");
?>

<?php
    if(!isset($doc_idx))
    {
        echo '수정할 글을 선택해주세요';
        exit();
    }

    $sql = "select * from bbs where doc_idx=$doc_idx";
    $query_result = $mysqli->query($sql);
    $data = $query_result->fetch_array();
?>

<form name="modify_form" method="POST" action="./modify_process.php">
    <input type="hidden" name="doc_idx" value="<?=$doc_idx?>">
    <table class=ModifyTableStyle>
        <tr class=ModifyTrStyle>
            <td class=ModifyTdStyle>제목</td>
            <td class=ModifyTdStyle><input type="text" name="subject" size="100" value="<?=$data['subject'];?>"></td>
        </tr>
        <tr class=ModifyTrStyle>
            <td class=ModifyTdStyle>내용</td>
            <td class=ModifyTdStyle><textarea name="content" cols="100" rows="10"><?=$data['content'];?></textarea></td>
        </tr>
    </table>
    <div>
        <input type="submit" value="저장">
    </div>
</form>

<?php  
    include_once($_SERVER['DOCUMENT_ROOT']."/footer.php");
?>