using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;

public class DayTimer : MonoBehaviour
{
    public float days = 1f;
    public TextMeshProUGUI daysText;
    Squad squad;

    WorldGenerator generator;
    EventController eventer; 

    public List<Cell> newlyExploredCells = new();
    public List<ShopData> shopsToProvisionRestoration = new();
    public List<Town> towns = new();

    public float oldDay;

    public void SetDays(int days)
    {
        oldDay = days;
        this.days = days;
        daysText.text = "Δενό: " + Mathf.FloorToInt(this.days).ToString();
    }
    public int GetDays()
    {
        return Mathf.FloorToInt(this.days);
    }
    private void Start()
    {
        squad = GameObject.Find("squad").GetComponent<Squad>();
        generator = GameObject.Find("generator").GetComponent<WorldGenerator>();
        eventer = GameObject.Find("Eventer").GetComponent<EventController>();
    }

    public void SpendTime(float days)
    {
        this.days += days;

        int oldD = Mathf.FloorToInt(oldDay);
        int newD = Mathf.FloorToInt(this.days);

        if (oldD != newD)
        {
            int deltaDays = newD - oldD;
            squad.UseProvision(deltaDays);
            if(squad.provision <= 25)
                eventer.AddEventToQueue(eventer.GetEventByID("need_provision"));
            foreach (Card card in squad.data.cards)
            {
                List<Effect> effectsToRemove = new List<Effect>();
                foreach (Effect eff in card.effects)
                {
                    EffectInfo effInfo = squad.info.effects[eff.id];
                    foreach (EffectAction action in effInfo.actions)
                    {
                        if (action.type == EffectActopnType.DealingDamageEveryMove)
                        {
                            card.hp -= action.effectPower;
                            if (card.hp <= 0)
                                squad.data.cards.Remove(card);
                            
                            if (squad.inventoryMode == false)
                                squad.DisplaySquadCards();
                        }
                    }
                    if (eff.durationType == EffectTypeByDuration.ExpiringDays)
                    {
                        eff.expirationTimeNumber--;
                        if (eff.expirationTimeNumber < 0)
                            effectsToRemove.Add(eff);
                    }
                }
                if (effectsToRemove.Count > 0)
                {
                    foreach (Effect effToRemove in effectsToRemove)
                    {
                        card.RemoveEffect(effToRemove, squad.info);
                    }
                    if(squad.inventoryMode == false)
                    {
                        squad.DisplaySquadCards();
                    }
                }
                    
                
            }
            
            foreach(Town town in towns)
            {
                town.daysToChangeStage -= deltaDays;
                if (town.daysToChangeStage <= 0)
                {
                    if(town.isSieged == false)
                        town.StartSiege(generator.world);
                    else
                        town.EndSiege(generator.world);
                    generator.DisplayWorld(generator.world);
                }
            }

            for (int i = newlyExploredCells.Count - 1; i >= 0; i--)
            {
                Cell cell = newlyExploredCells[i];
                cell.daysBeforeUpdate -= deltaDays;

                if (cell.daysBeforeUpdate <= 0)
                {
                    cell.daysBeforeUpdate = 0;
                    newlyExploredCells.RemoveAt(i);
                }
            }
            if (newD % 4 == 0)
            {
                foreach (ShopData shop in shopsToProvisionRestoration)
                {
                    if (shop.GetCountOf(12) <= 8)
                    {
                        shop.goods.Add(new Item(12, 5));
                        shop.goods.Add(new Item(12, 5));
                    }
                }
            }


        }

        oldDay = this.days;
        daysText.text = "Δενό: " + newD.ToString();
    }
}
