VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsList"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Private m_front As clsNode
Private m_back As clsNode

Public Property Get Front() As clsNode
    Set Front = m_front
End Property

Public Property Get Back() As clsNode
    Set Back = m_back
End Property

Public Sub PushFront(objNode As clsNode)

    If m_front Is Nothing Then
    
        Set objNode.PrevNode = Nothing
        Set objNode.NextNode = Nothing
        
        Set m_front = objNode
        Set m_back = objNode
        
    Else
    
        Set objNode.PrevNode = Nothing
        Set objNode.NextNode = m_front
        Set m_front.PrevNode = objNode
        
        Set m_front = objNode
        
    End If
    
End Sub

Public Sub PushBack(objNode As clsNode)

    If m_front Is Nothing Then
    
        Set objNode.PrevNode = Nothing
        Set objNode.NextNode = Nothing
        
        Set m_front = objNode
        Set m_back = objNode
        
    Else
    
        Set objNode.PrevNode = m_back
        Set objNode.NextNode = Nothing
        Set m_back.NextNode = objNode
        
        Set m_back = objNode
        
    End If
    
End Sub

Public Sub Insert(objSibling As clsNode, objNode As clsNode)

    If objSibling Is Nothing Then
        
        PushBack objNode
        
    ElseIf objSibling Is m_front Then
        
        PushFront objNode
        
    Else
    
        Set objNode.PrevNode = objSibling.PrevNode
        Set objNode.NextNode = objSibling
        Set objSibling.PrevNode.NextNode = objNode
        Set objSibling.PrevNode = objNode
        
    End If
    
End Sub

Public Sub Remove(objNode As clsNode)

    Dim curNode As clsNode
    
    Set curNode = m_front
    Do Until curNode Is Nothing
    
        If curNode Is objNode Then
        
            If Not curNode.PrevNode Is Nothing Then
                Set curNode.PrevNode.NextNode = curNode.NextNode
            Else
                Set m_front = curNode.NextNode
            End If
            
            If Not curNode.NextNode Is Nothing Then
                Set curNode.NextNode.PrevNode = curNode.PrevNode
            Else
                Set m_back = curNode.PrevNode
            End If
            
        End If
        
        Set curNode = curNode.NextNode
    Loop
    
End Sub

Public Function CreateAnyShapeNode() As clsNode

    Set CreateAnyShapeNode = New clsNode
    
    Set CreateAnyShapeNode.NextNode = Nothing
    Set CreateAnyShapeNode.PrevNode = Nothing
    Set CreateAnyShapeNode.Value = New clsAnyShape
    
End Function

