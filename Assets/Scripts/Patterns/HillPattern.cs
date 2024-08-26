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
        string specialDescription = "Может быть использовано одним наёмником (в том числе в бою).\n" + "Восстанавливет от " + hill.minHilledHp.ToString() + " до " + hill.maxHilledHp.ToString() + " здоровья";
        return baseDescription.Length > 0 ? baseDescription + "\n" + specialDescription : specialDescription;
    }
}
