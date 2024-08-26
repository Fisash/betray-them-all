using System.Collections;
using System.Collections.Generic;
using UnityEngine;


[CreateAssetMenu(menuName = "Custom/HillPattern")]
public class HillPattern : ItemPattern
{
    public int minHilledHp;
    public int maxHilledHp;


    public override string GetDescription(Item item)
    {
        Hill hill = (Hill)item;
        string specialDescription = "����� ���� ������������ ����� �������� (� ��� ����� � ���).\n" + "�������������� �� " + hill.minHilledHp.ToString() + " �� " + hill.maxHilledHp.ToString() + " ��������";
        return baseDescription.Length > 0 ? baseDescription + "\n" + specialDescription : specialDescription;
    }
}
