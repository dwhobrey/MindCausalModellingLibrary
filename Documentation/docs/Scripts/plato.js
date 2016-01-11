//---------------------------//
//     TREE BROWSER CODE     //
// Copyright Andrew Vos 2006 //
//---------------------------//

var parentNodeCount = 0;
var nodeCount = 0;
var sectionNodeCount = 0;

function onParentNodeImageClick(node) {
    var divNode = document.getElementById(node.name.replace("parentNodeImage","parentNodeDiv"));
	var imageNode = node;

 	if (divNode.style.display == "none"){
		imageNode.src = "../images/minus.png";
		divNode.style.display = "";
	}
	else {
		imageNode.src = "../images/plus.png";
		divNode.style.display = "none";
	}
}
function onParentNodeTextClick(node) {
    var divNode = document.getElementById(node.name.replace("parentNodeText","parentNodeDiv"));
    var imageNode = document.getElementsByName(node.name.replace("parentNodeText","parentNodeImage"))[0];

	if (divNode.style.display == 'none') {
		imageNode.src = "../images/minus.png";
		divNode.style.display = '';
	}
	else {
		imageNode.src = "../images/plus.png";
		divNode.style.display = 'none';
	}
}

function onNodeTextClick(node) {
    var imageNode = document.getElementsByName(node.name.replace("nodeText","nodeImage"))[0];
    setSelectedNode(imageNode);
}
function onNodeImageClick(node, url, target){
    var imageNode = node
    setSelectedNode(imageNode);
}

function setSelectedNode(imageNode){
    for (index = 0; index < this.nodeCount; index++) {
		document.getElementsByName("nodeImage" + index)[0].src = "../images/page.png";
    }
   	imageNode.src = "pageSelected.png";
}
function expandAll(){
	for (index = 0; index < this.parentNodeCount; index++) {
		document.getElementById("parentNodeDiv" + index).style.display = "";
        document.getElementsByName("parentNodeImage" + index)[0].src = "../images/minus.png";
	}
}
function collapseAll(){
	for (index = 0; index < this.parentNodeCount; index++) {
		document.getElementById("parentNodeDiv" + index).style.display = "none";
        document.getElementsByName("parentNodeImage" + index)[0].src = "../images/plus.png";
	}
}

function startParentNode(text){
	document.write('<table border="0" cellpadding="1" cellspacing="0">');
	document.write('  <tr>');
	document.write('    <td><img src="../images/plus.png" name="parentNodeImage' + parentNodeCount
           + '" onclick="onParentNodeImageClick(this)" style="cursor:pointer;"/></td>');
	document.write('    <td><a class="parentTreeNode" name="parentNodeText' + parentNodeCount
           + '" onclick="onParentNodeTextClick(this)" style="cursor:pointer;">');
	document.write(text);
	document.write('</a>');
	document.write('  </td>');
	document.write('  </tr>');
	document.write('  <tr>');
	document.write('    <td></td><!-- SPACING -->');
	document.write('	<td><DIV id="parentNodeDiv' + parentNodeCount + '" style="display:none">');
    this.parentNodeCount = this.parentNodeCount + 1;
}
function endParentNode(){
	document.write('</DIV></td>');
	document.write('  </tr>');
	document.write('</table>');
}
function addNode(text, url, target){
	document.write('<table border="0" cellpadding="1" cellspacing="0">');
	document.write('  <tr>');
	document.write('	<td>');
    document.write('<a href="' + url + '" target="' + target + '" onfocus="this.hideFocus=true;" style="outline-style:none;">');
	document.write('<img src="../images/page.png" border="0" name="nodeImage' + this.nodeCount + '" onclick="onNodeImageClick(this);" />');
    document.write('</a>');
	document.write('	</td>');
	document.write('    <td><a name="nodeText' + this.nodeCount + '" onclick="onNodeTextClick(this);" href="' + url + '" target="' + target + '" class="normalTreeNode" onfocus="this.hideFocus=true;" style="outline-style:none;">' + text + '</a></td>');
	document.write('  </tr>');
	document.write('</table>');
    this.nodeCount = this.nodeCount + 1;
}
function addExpandCollapseAll(){
	document.write('<table width="100%" border="0">');
	document.write('  <tr>');
	document.write('    <td align="right" width="50%"><a onclick="expandAll();" class="expandCollapse" style="cursor:pointer;">Expand All</a></td>');
	document.write('    <td alight="left" width="50%"><a onclick="collapseAll();" class="expandCollapse" style="cursor:pointer;">Collapse All</a></td>');
	document.write('  </tr>');
	document.write('</table>');
}

// Section Expansion scripts

function onSectionNodeClick(sectionName) {
    var divNode = document.getElementById(sectionName);
    var imageNode = document.getElementsByName(sectionName + "Image")[0];

	if (divNode.style.display == "none") {
		imageNode.src = "../images/sectionMinus.gif";
		divNode.style.display = "";
	}
	else {
		imageNode.src = "../images/sectionPlus.gif";
		divNode.style.display = "none";
	}
}
function openSection(sectionName) {
    var divNode = document.getElementById(sectionName);
    var imageNode = document.getElementsByName(sectionName + "Image")[0];

	if (divNode.style.display == "none") {
		imageNode.src = "../images/sectionMinus.gif";
		divNode.style.display = "";
	}

}

function startSection(level,style,text,sectionName){
        document.write('<' + level + ' class="' + style + '">');
	document.write('<img src="../images/sectionPlus.gif" class="toggle" name="'+sectionName + 'Image'
           + '" onclick="onSectionNodeClick(\''+ sectionName + '\')" style="cursor:pointer;"/>');
        document.write('<span name="'+sectionName + 'Text'
           + '" onclick="onSectionNodeClick(\''+ sectionName + '\')" style="cursor:pointer;">');
	document.write(text);
	document.write('</span></h1> <DIV id="'+sectionName+'" style="display:none;">');
        this.sectionNodeCount = this.sectionNodeCount + 1;
}
function endSection(){
	document.write('</DIV>');
}

// Copy Code
function CopyCode(key)
{
	var trElements = document.all.tags("tr");
	var i;
	for(i = 0; i < trElements.length; ++i)
	{
		if(key.parentElement.parentElement.parentElement == trElements[i].parentElement)
		{
			window.clipboardData.setData("Text", trElements[i].innerText);
		}
	}
}

function ChangeCopyCodeIcon(key)
{
	var i;
	var imageElements = document.getElementsByName("ccImage")
	for(i=0; i<imageElements.length; ++i)
	{
		if(imageElements[i].parentElement == key)
		{
			if(imageElements[i].src == copyImage.src)
			{
				imageElements[i].src = copyHoverImage.src;
				imageElements[i].alt = copyHoverImage.alt;
			}
			else
			{
				imageElements[i].src = copyImage.src;
				imageElements[i].alt = copyImage.alt;
			}
		}
	}
}
