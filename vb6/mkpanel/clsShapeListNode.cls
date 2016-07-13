VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsNode"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Private m_value As Object
Private m_next As clsNode
Private m_prev As clsNode

Public Property Get NextNode() As clsNode
    Set NextNode = m_next
End Property

Public Property Set NextNode(objNode As clsNode)
    Set m_next = objNode
End Property

Public Property Get PrevNode() As clsNode
    Set PrevNode = m_prev
End Property

Public Property Set PrevNode(objNode As clsNode)
    Set m_prev = objNode
End Property

Public Property Get Value() As Object
    Set Value = m_value
End Property

Public Property Set Value(objValue As Object)
    Set m_value = objValue
End Property
