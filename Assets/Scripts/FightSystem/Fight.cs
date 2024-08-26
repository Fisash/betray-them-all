using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.Serialization.Formatters.Binary;
using UnityEngine;
using TMPro;
using UnityEngine.UI;

public class Fight : MonoBehaviour, ITapItemHandler
{
    public Skill skipSkill;

    public TextMeshProUGUI fightStageHint;

    public TextMeshProUGUI logText;
    ScrollRect scrollRect;
    public FightData data;

    public Sprite emptyCardIcon;

    public Transform playerCardsParent;
    public Transform enemyCardsParent;

    private CardFightPrefab[] playerCardPrefabs;
    private CardFightPrefab[] enemyCardPrefabs;

    public ItemsAndClassesInfo info;

    public bool selectionCardMode = false;

    [HideInInspector]
    public Card currentMovementCard;

    [HideInInspector]
    public Skill selectedSkill;

    public Transform skillsPanel;
    public GameObject skillSlotPrefab;

    public GameObject skillHintPanel;

    public Transform itemsContent;
    public GameObject itemPrefab;
    public GameObject itemHint;

    TextMeshProUGUI skillnameText;
    TextMeshProUGUI skilldescriptionText;

    public string loadPath = @"C:\Games\BetrayThemAll\current.fight";

    public List<Card> playerCardsToMove;
    public List<Card> enemyCardsToMove;

    List<Card> currentCardsToMove;

    public Transform endFightPanelTransform;

    EndFightPanel endFightPanel;

    SaveData saveData;

    public ScenChanger scenChanger;

    public FightStatus fightStatus = FightStatus.Active;

    public int addedGold;

    public float fightSpeed = 1f;
    string losses = "";
    AudioSource sounder;
    public AudioClip miss;
    public AudioClip lose;
    public AudioClip victory;

    public AudioClip bossFightTheme;

    public void ChangeFightSpeed(TextMeshProUGUI changeFightSpeedButtonText)
    {
        switch (fightSpeed)
        {
            case 1f:
                fightSpeed = 1.5f;
                break;
            case 1.5f:
                fightSpeed = 2f;
                break;
            case 2f:
                fightSpeed = 4f;
                break;
            case 4f:
                fightSpeed = 1f;
                break;
        }
        changeFightSpeedButtonText.text = "—корость бо€ " + fightSpeed.ToString("0.0") + "’";
    }
    public void Initialize()
    {
        endFightPanel = new EndFightPanel(endFightPanelTransform);
        sounder = GameObject.Find("SoundManager").GetComponent<AudioSource>();
        skillnameText = skillHintPanel.transform.GetChild(0).GetComponent<TextMeshProUGUI>();
        skilldescriptionText = skillHintPanel.transform.GetChild(1).GetComponent<TextMeshProUGUI>();
        scrollRect = logText.transform.parent.parent.parent.parent.GetComponent<ScrollRect>();
        playerCardPrefabs = new CardFightPrefab[10];
        for (int i = 0; i < playerCardPrefabs.Length; i++)
            playerCardPrefabs[i] = GetCardSlot(i, false);
        enemyCardPrefabs = new CardFightPrefab[10];
        for (int i = 0; i < enemyCardPrefabs.Length; i++)
            enemyCardPrefabs[i] = GetCardSlot(i, true);
    }
    public void TapItem(Item item)
    {
        if (selectionCardMode)
            return;
        if (item is Hill)
        {
            Hill hill = (Hill)item;
            int hilledHp = Random.Range(hill.minHilledHp, hill.maxHilledHp + 1);
            currentMovementCard.GetTakingHillResult(hilledHp);
            GetCardSlot(currentMovementCard).animator.Play("getting_hill");
        }
        saveData.squad.inventory.Remove(item);
        itemHint.gameObject.SetActive(false);
        ShowItems();
        NextMoveStep();
    }
    public void ClearItemsContent()
    {
        foreach(Transform child in itemsContent)
        {
            Destroy(child.gameObject);
        }
    }

    public void SetCardAnimatorsActive(bool isActive)
    {
        foreach (var slot in playerCardPrefabs)
            slot.animator.enabled = isActive;
        foreach (var slot in enemyCardPrefabs)
            slot.animator.enabled = isActive;
    }
    public void ShowItems()
    {
        ClearItemsContent();
        foreach (Item item in saveData.squad.inventory)
        {
            if (info.items[item.patternID].isItUsableInFight)
            {
                GameObject newItemPrefab = Instantiate(itemPrefab, itemsContent);
                ItemPrefab slot = newItemPrefab.GetComponent<ItemPrefab>();
                slot.SetType(ItemPrefabType.FightItem);
                slot.SetDisplay(item);
            }
        }
    }
    public CardFightPrefab GetCardSlot(int index, bool isEnemy)
    {
        Transform cardsParent = isEnemy ? enemyCardsParent : playerCardsParent;

        foreach (Transform child in cardsParent)
        {
            CardFightPrefab prefab = child.GetComponent<CardFightPrefab>();
            if (prefab.index == index)
                return prefab;
        }

        return null;
    }
    public (AttackResult, int damage) PerformAttack(Card attacker, Card target, float damageIndex = 1f)
    {
        GetCardSlot(attacker).animator.Play("atack");
        int damage = Mathf.RoundToInt(attacker.GetRandNormalDamage() * damageIndex);
        AttackResult res = AttackResult.SuccessfulAttack;

        if (attacker.IsCriticalStrikeAtack())
        {
            AddToLog("Ёто была критическа€ атака.");
            damage *= 2;
            res =  AttackResult.CriticalAttack;
        }
        else
            res = AttackResult.SuccessfulAttack;

        if (Random.Range(1, 101) <= target.GetEvasionChance())
        {
            AddToLog(target.name + " уклонилс€ от атаки");
            return (AttackResult.Evasion, 0);
        }

        int damageWithProtection = damage - Random.Range(target.GetProtection().x, target.GetProtection().y + 1);
        if (damageWithProtection < 0)
        {
            damageWithProtection = 0;
        }
        target.hp -= damageWithProtection;
        AddToLog(target.name + " получил " + damageWithProtection.ToString() + " урона");
        GetCardSlot(target).animator.Play("getting_hit");
        if (target.hp <= 0)
        {
            attacker.addedExp += target.exp;
            playerCardsToMove.Remove(target);
            enemyCardsToMove.Remove(target);
            Kill(target);
        }

        return (res, damageWithProtection);
    }
    
    public void DoSkill(Skill skill, Card attacker, Card targetCard)
    {
        attacker.addedExp += Mathf.RoundToInt(skill.baseGivenExp * Random.Range(0.8f, 1.3f));
        AddToLog(attacker.name + " примен€ет " + skill.skillName);

        bool isPlayerTurn = (currentCardsToMove == playerCardsToMove);

        Card[] currentAlliesCards = isPlayerTurn ? data.playerCards : data.enemyCards;
        Card[] currentEnemyesCards = isPlayerTurn ? data.enemyCards : data.playerCards;

        AttackResult attackResult = AttackResult.SuccessfulAttack;
        int damage = 0;
        switch (skill.id)
        {
            case 1: // обычна€ атака
            case 4: // дальнобойна€ атака
            case 5: // порез (накладывает кровотечение)
                (attackResult, damage) = PerformAttack(attacker, targetCard);
                break;
            case 12: //супер удар танка
                (attackResult, damage) = PerformAttack(attacker, targetCard, 1.9f);
                attacker.ApplyEffect(new Effect(8, EffectTypeByDuration.ExpiringMoves, 1), info);
                break;
            case 2: // лечение обычное
                int healPoints = Random.Range(3, 6);
                if (attacker.IsCriticalStrikeAtack())
                {
                    AddToLog("Ёто было критическое излечение");
                    healPoints *= 2;
                }
                GetCardSlot(targetCard).animator.Play("getting_hill");
                AddToLog(targetCard.GetTakingHillResult(healPoints));
                break;
            case 10: //атака по линии лучника

                if(GetCardSlot(targetCard) == null)
                {
                    break;
                }
                int index = GetCardSlot(targetCard).index;
                if(index <= 4)
                {
                    for(int i = 0; i <= 4; i++)
                    {
                        if(GetCardSlot(i, isPlayerTurn).card != null && GetCardSlot(i, isPlayerTurn).card.isNotEmpty)
                            (attackResult, damage) = PerformAttack(attacker, GetCardSlot(i, isPlayerTurn).card, 0.6f);
                    }
                    
                }
                else
                {
                    for (int i = 5; i <= 9; i++)
                    {
                        if (GetCardSlot(i, isPlayerTurn).card != null && GetCardSlot(i, isPlayerTurn).card.isNotEmpty)
                            (attackResult, damage) = PerformAttack(attacker, GetCardSlot(i, isPlayerTurn).card, 0.6f);
                    }
                }
                
                break;
            case 13: //лечение всех
                bool isCrit = false;
                if (attacker.IsCriticalStrikeAtack())
                {
                    AddToLog("Ёто было критическое излечение");
                    isCrit = true;
                }
                foreach (Card card in currentAlliesCards) {
                    if (card != null && card.isNotEmpty)
                    {
                        healPoints = Random.Range(1, 3);
                        if (isCrit)
                        {
                            healPoints *= 2;
                        }
                        GetCardSlot(card).animator.Play("getting_hill");
                        AddToLog(card.GetTakingHillResult(healPoints));
                    }
                }
                break;
            case 3: // пропуск хода
                break;
            case 6: //призыв кучи пепла у духа
                Card minion = new Card(8, info, !isPlayerTurn);
                minion.leader = attacker;
                SpawnCard(minion, !isPlayerTurn);
                break;
            case 7: //призыв нежити у вампира  
                for (int i = 0; i < 2; i++)
                {
                    minion = new Card(10, info, !isPlayerTurn);
                    minion.leader = attacker;
                    SpawnCard(minion, !isPlayerTurn);
                }
                break;
            case 16: //призыв скелетов у лича
                for (int i = 0; i < Random.Range(4, 6); i++)
                {
                    minion = new Card(11, info, !isPlayerTurn);
                    minion.leader = attacker;
                    SpawnCard(minion, !isPlayerTurn);
                }
                break;
            case 17: //лечение за счет союзников лича
                int sumHillPoints = 0;
                foreach (Card card in currentAlliesCards)
                {
                    if (card != null && card.isNotEmpty && card != attacker)
                    {
                        int maxHealForCard = card.hp;
                        (attackResult, damage) = PerformAttack(attacker, card);
                        GetCardSlot(attacker).animator.Play("getting_hill");
                        if(damage > maxHealForCard)
                        {
                            damage = maxHealForCard;
                        }
                        sumHillPoints += Mathf.RoundToInt(damage * 0.75f);
                    }
                }
                AddToLog(attacker.GetTakingHillResult(sumHillPoints));
                break;
            case 8: //ужасный вой вампира
                foreach (Card card in currentEnemyesCards)
                {
                    if (card != null && card.isNotEmpty)
                    {
                        (attackResult, damage) = PerformAttack(attacker, card, 0.6f);
                        if(attackResult != AttackResult.Evasion)
                        card.ApplyEffect(new Effect(5, EffectTypeByDuration.ExpiringMoves, 2), info);
                    }
                }
                break;
            case 9: //лечение вампира
                (attackResult, damage) = PerformAttack(attacker, targetCard, 1.2f);
                GetCardSlot(attacker).animator.Play("getting_hill");
                AddToLog(attacker.GetTakingHillResult(Mathf.RoundToInt(damage*0.8f)));
                break;
            case 14: //боевой дух воина
                foreach (Card card in currentAlliesCards)
                {
                    if (card != null && card.isNotEmpty)
                        card.ApplyEffect(new Effect(9, EffectTypeByDuration.ExpiringMoves, 4), info);
                }
                break;
        }

        if (attackResult != AttackResult.Evasion)
        {
            if(skill.sound != null && skill.sound.Length > 0)
                sounder.PlayOneShot(skill.sound[Random.Range(0, skill.sound.Length)]);
            foreach (ApplyingEffect eff in skill.effects)
            {
                if (Random.Range(0, 101) <= eff.applyChance)
                {
                    AddToLog(targetCard.name + " получает " + info.effects[eff.effect.id].title);
                    targetCard.ApplyEffect(new Effect(eff.effect.id, eff.effect.durationType, eff.effect.expirationTimeNumber), info);
                }
            }
        }
        else
            sounder.PlayOneShot(miss);

        UpdateDisplayCards();
    }

    public void SpawnCard(Card card, bool isEnemy)
    {
        int index = 0;
        CardFightPrefab[] slots = isEnemy ? enemyCardPrefabs : playerCardPrefabs;
        while (slots[index].isEnemy == false || slots[index].card != null)
        {
            index++;
            if(index > 9)
                return;
        }

        CardFightPrefab slot = GetCardSlot(index, isEnemy);
        slot.card = card;
        slot.UpdateDisplay();
        if (isEnemy)
            data.enemyCards[index] = card;

        else
            data.playerCards[index] = card;
       
        UpdateDisplayCards();
    }
    public void StartMove(bool randomStarter = false)
    {
        if (fightStatus != FightStatus.Active)
            return;
        playerCardsToMove = GetNotNullCards(data.playerCards);
        enemyCardsToMove = GetNotNullCards(data.enemyCards);
        
        var randMoveCards = Random.Range(0, 2) == 0 ? playerCardsToMove : enemyCardsToMove;
        currentCardsToMove = randomStarter ? randMoveCards : playerCardsToMove;

        currentMovementCard = currentCardsToMove[Random.Range(0, currentCardsToMove.Count)];
        ApplyEffects();

        if (currentCardsToMove == playerCardsToMove)
            ShowSkills(currentMovementCard);
        else
            EnemyAtack();
    }

    public void EnemyAtack()
    {
        CardFightPrefab movementer = GetCardSlot(currentMovementCard);
        List<Skill> skillsToMove = GetUsefulSkills(movementer);
        skillsToMove.Remove(skipSkill);
        if (skillsToMove.Count == 0)
            skillsToMove.Add(skipSkill);
        Skill skill = skillsToMove[Random.Range(0, skillsToMove.Count)];

        bool skillForAll = skill.typeByTarget == TypeByTarget.AllEnemyes || skill.typeByTarget == TypeByTarget.AllAlly || skill.typeByTarget == TypeByTarget.Self;

        List<CardFightPrefab> cardsToApplySkill = GetCardPrefabsToApplySkill(skill.typeByTarget, movementer);
        DoSkill(skill, currentMovementCard, skillForAll ? currentMovementCard : ChooseTargetForSkill(skill, cardsToApplySkill));
        Invoke("NextMoveStep", 1f / fightSpeed);
    }
    
    private List<Skill> GetUsefulSkills(CardFightPrefab movementer)
    {
        List<Skill> result = GetSuitableSkills(movementer);

        bool isPlayerTurn = (currentCardsToMove == playerCardsToMove);
        Card[] currentAlliesCards = isPlayerTurn ? data.playerCards : data.enemyCards;
        Card[] currentEnemyesCards = isPlayerTurn ? data.enemyCards : data.playerCards;

        if (movementer.card.HaveIEffect(9))//командный дух воина
            result.Remove(info.GetSkillByID(14));
        if (movementer.card.HaveIEffect(7))//неистовство танка
            result.Remove(info.GetSkillByID(11));
        if (movementer.card.HaveIEffect(10))//тень вора 
            result.Remove(info.GetSkillByID(15));
        if (GetNotNullCards(currentAlliesCards).Count >= 7)
        {
            result.Remove(info.GetSkillByID(16));
            result.Remove(info.GetSkillByID(7));
        }
        if (GetNotNullCards(currentAlliesCards).Count < 3)
            result.Remove(info.GetSkillByID(17));
        return result;

    }

    private Card ChooseTargetForSkill(Skill skill, List<CardFightPrefab> cardsToApplySkill)
    {
        switch (skill.enemyStrategy)
        {
            case EnemyStrategy.ChooseWithLessHP:
                return GetLeastStatCard(cardsToApplySkill, 0);
            default:
                return cardsToApplySkill[Random.Range(0, cardsToApplySkill.Count)].card;
        }
    }

    private Card GetLeastStatCard(List<CardFightPrefab> cardsToApplySkill, int statID)
    {
        Card result = cardsToApplySkill[0].card;
        for(int i = 0; i < cardsToApplySkill.Count; i++)
        {
            bool[] conditions = new bool[] { (cardsToApplySkill[i].card.hp < result.hp) };
            if (conditions[0])
                result = cardsToApplySkill[i].card;
        }
        return result;
    }
    public void NextMoveStep()
    {
        if (fightStatus != FightStatus.Active)
        {
            return;
        }
        SetCardAnimatorsActive(true);
        UpdateDisplayCards();
        currentCardsToMove.Remove(currentMovementCard);
        if(currentCardsToMove.Count == 0)
        {
            if(currentCardsToMove == enemyCardsToMove)
            {
                StartMove();
                return;
            }
            currentCardsToMove = enemyCardsToMove;
        }
        currentMovementCard = currentCardsToMove[Random.Range(0, currentCardsToMove.Count)];

        ApplyEffects();

        if (currentCardsToMove == playerCardsToMove)
            ShowSkills(currentMovementCard);
        else if (currentCardsToMove == enemyCardsToMove)
        {
            ClearSkillContent();
            fightStageHint.text = "—ейчас ходит " + currentMovementCard.name;
            Invoke("EnemyAtack", 0.5f / fightSpeed);
        }
    }
    public void ApplyEffects()
    {
        List<Effect> effectsToRemove = new List<Effect>();
        foreach (Effect eff in currentMovementCard.effects)
        {
            EffectInfo effInfo = info.effects[eff.id];
            foreach (EffectAction action in effInfo.actions)
            {
                if (action.type == EffectActopnType.DealingDamageEveryMove && eff.expirationTimeNumber > 0)
                {
                    currentMovementCard.hp -= action.effectPower;
                    GetCardSlot(currentMovementCard).animator.Play("getting_hit");
                    AddToLog(currentMovementCard.name + " тер€ет " + action.effectPower.ToString() + " здоровь€ от " + effInfo.title);
                    UpdateDisplayCards();
                    if (currentMovementCard.hp <= 0)
                    {
                        playerCardsToMove.Remove(currentMovementCard);
                        enemyCardsToMove.Remove(currentMovementCard);
                        Kill(currentMovementCard);
                        NextMoveStep();
                        return;
                    }
                }
            }
            if (eff.durationType == EffectTypeByDuration.ExpiringMoves)
            {
                eff.expirationTimeNumber--;
                if (eff.expirationTimeNumber < 0)
                    effectsToRemove.Add(eff);
            }
        }
        foreach (Effect effToRemove in effectsToRemove)
            currentMovementCard.RemoveEffect(effToRemove, info);
    }
    public void TapCard(CardFightPrefab card) 
    {
        if (selectionCardMode && currentCardsToMove != enemyCardsToMove)
        {
            DoSkill(selectedSkill, currentMovementCard, card.card);
            NextMoveStep();
        }
    }

    public void TapSkill(Skill skill)
    {
        if (!selectionCardMode)
        {
            selectedSkill = skill;
            selectionCardMode = true;
            if(skill.typeByTarget == TypeByTarget.AllAlly || skill.typeByTarget == TypeByTarget.AllEnemyes || skill.typeByTarget == TypeByTarget.Self)
            {
                DoSkill(selectedSkill, currentMovementCard, currentMovementCard);
                NextMoveStep();
                return;
            }
            fightStageHint.text = "¬ыберите на ком " + currentMovementCard.name + " применит " + skill.skillName;
            SetCardsBacklight(skill.typeByTarget, GetCardSlot(currentMovementCard));
        }
    }

    public List<CardFightPrefab> GetCardPrefabsToApplySkill(TypeByTarget skillType, CardFightPrefab movementer)
    {

        CardFightPrefab[] allyCards = !movementer.isEnemy ? playerCardPrefabs : enemyCardPrefabs;
        CardFightPrefab[] enemyCards = !movementer.isEnemy ? enemyCardPrefabs : playerCardPrefabs;

        List<CardFightPrefab> result = new List<CardFightPrefab>();

        switch (skillType)
        {
             case TypeByTarget.OneEnemyNearLine:
                 for (int i = 0; i < 5; i++)
                 {
                     if (enemyCards[i].card != null && enemyCards[i].card.isNotEmpty)
                     {
                        result.Add(enemyCards[i]);
                     }
                     else if (enemyCards[i + 5].card != null && enemyCards[i + 5].card.isNotEmpty)
                     {
                        result.Add(enemyCards[i+5]);
                     }
                 }
                break;
             case TypeByTarget.OneEnemy:
                foreach (var slot in enemyCards)
                {
                    if (slot.card != null && slot.card.isNotEmpty)
                    {
                        result.Add(slot);
                    }
                }
                break;
             case TypeByTarget.OneAlly:
                foreach (var slot in allyCards)
                {
                    if (slot.card != null && slot.card.isNotEmpty)
                    {
                        result.Add(slot);
                    }
                }
                break;
             default:
                 break;
        }
        return result;
    }
    public void SetCardsBacklight(TypeByTarget type, CardFightPrefab movementer)
    {
        SetCardAnimatorsActive(false);
        ResetAllSlots();
        List<CardFightPrefab> toBackLight = GetCardPrefabsToApplySkill(type, movementer);
        foreach (CardFightPrefab slot in toBackLight)
        {
            slot.SetBacklight(true);
        }
    }
    private void ShowAllSlots()
    {
        foreach (var slot in playerCardPrefabs)
            slot.SetBacklight(true);
        foreach (var slot in enemyCardPrefabs)
            slot.SetBacklight(true);
    }
    private void ResetAllSlots()
    {
        foreach (var slot in playerCardPrefabs)
            slot.SetBacklight(false);
        foreach (var slot in enemyCardPrefabs)
            slot.SetBacklight(false);
    }

    public void ShowSkillHint(Skill skill)
    {
        skillHintPanel.SetActive(true);
        skillnameText.text = skill.skillName;
        skilldescriptionText.text = skill.descriprion;
    }
    public void HideSkillHint()
    {
        skillHintPanel.SetActive(false);
    }
    public List<Card> GetNotNullCards(Card[] cards)
    {
        List<Card> result = new List<Card>();

        for(int i = 0; i < cards.Length; i++)
        {
            if (cards[i] != null && cards[i].isNotEmpty)
                result.Add(cards[i]);
        }
        return result;
    }
    public List<Card> GetNotNullCards(CardFightPrefab[] slots)
    {
        List<Card> result = new List<Card>();

        for (int i = 0; i < slots.Length; i++)
        {
            if (slots[i].card != null && slots[i].card.isNotEmpty)
                result.Add(slots[i].card);
        }
        return result;
    }
    public void ShowSkills(Card card)
    {
        ShowAllSlots();
        ClearSkillContent();
        List<Skill> suitableSkills = GetSuitableSkills(GetCardSlot(card));

        foreach (Skill skill in info.GetClassPatternByType(card.type).skills)
        {
            if(card.level < skill.requiredLevel)
            {
                continue;
            }
            GameObject newSkillSlot = Instantiate(skillSlotPrefab, skillsPanel);
            SkillSlot slot = newSkillSlot.GetComponent<SkillSlot>();
            slot.UpdateDisplay(skill);
            slot.SetBacklight(suitableSkills.Contains(skill));
        }

        selectionCardMode = false;
        fightStageHint.text = "¬ыберите навык дл€ хода " + card.name;
    }

    public List<Skill> GetSuitableSkills(CardFightPrefab card)
    {
        List<Skill> result = new List<Skill>();

    
        foreach (Skill skill in info.GetClassPatternByType(card.card.type).skills)
        {
            if(skill.requiredLevel > card.card.level)
                continue;

            switch (skill.atackerPos)
            {
                case TypeByAttackerPosition.AnyPos:
                    result.Add(skill);
                    break;
                case TypeByAttackerPosition.OnlyFirstLine:
                    CardFightPrefab[] allyCards = card.isEnemy ? enemyCardPrefabs : playerCardPrefabs;
                    if (card.index <= 4 || (allyCards[card.index-5].card == null || allyCards[card.index-5].card.isNotEmpty == false))
                        result.Add(skill);
                    break;
                default:
                    break;
            }
        }
        return result;
    }
    public void ClearSkillContent()
    {
        foreach (Transform child in skillsPanel)
            Destroy(child.gameObject);
    }
    public void Start()
    {
        loadPath = PlayerPrefs.GetString("loadPath");
        Initialize();
        //data = new FightData(new List<Card>() { Squad.CreateRandomNewMercenary(info), Squad.CreateRandomNewMercenary(info) }, new List<Card>() { new Card(1, info, true), new Card(1, info, true), new Card(1, info, true), new Card(1, info, true), new Card(1, info, true), new Card(1, info, true), new Card(1, info, true)});
        LoadFight();
        UpdateDisplayCards();
        ShowItems();
        StartMove(true);
    }

    public void CheckFollowers(Card leader)
    {
        for (int i = 0; i < data.enemyCards.Length; i++)
        {
            if (data.enemyCards[i] != null && data.enemyCards[i].isNotEmpty && data.enemyCards[i].leader == leader)
            {
                AddToLog(data.enemyCards[i].name + " растер€н из-за смерти лидера");
                data.enemyCards[i].ApplyEffect(new Effect(6, EffectTypeByDuration.Permanent, 999), info);
            }
        }
    }
    public void Kill(Card card)
    {
        for (int i = 0; i < data.playerCards.Length; i++)
        {
            if(data.playerCards[i] == card)
            {
                playerCardPrefabs[i].card = null;
                saveData.squad.cards.Remove(card);
                data.playerCards[i] = null;
                AddToLog(card.name + " умирает");
                losses += "\n" + card.name + " пал.";
                CheckFightStatus();
                return;
            }
        }

        for (int i = 0; i < data.enemyCards.Length; i++)
        {
            if (data.enemyCards[i] == card )
            {
                AddToLog(card.name + " умирает");
                CheckFollowers(data.enemyCards[i]);

                addedGold += (int)card.type >= info.classes.Length ? card.price : Random.Range(3, 6) ;
                enemyCardPrefabs[i].card = null;
                data.enemyCards[i] = null;
                CheckFightStatus();
                return;
            }
        }
        AddToLog(card.name + " уже мЄртв");
    }

    public void CheckFightStatus()
    {
        List<Card> playerCards = GetNotNullCards(playerCardPrefabs);
        if(playerCards.Count == 0)
        {
            sounder.PlayOneShot(lose);
            fightStatus = FightStatus.Lose;
            endFightPanel.ShowGameOver();
            return;
        }

        List<Card> enemyCard = GetNotNullCards(enemyCardPrefabs);
        if (enemyCard.Count == 0)
        {
            fightStatus = FightStatus.Victory;

            string expInfo = "";
            foreach (Card card in playerCards)
            {
                var effectsToRemove = card.effects.Where(eff => eff.durationType == EffectTypeByDuration.ExpiringMoves).ToList();
                foreach (Effect eff in effectsToRemove)
                {
                    card.RemoveEffect(eff, info);
                }

                int oldLevel = card.level;
                card.AddExp(card.addedExp);
                expInfo += "\n" + card.name + " получил " + card.addedExp + " опыта.";
                if(oldLevel < card.level)
                {
                    for (int i = oldLevel + 1; i <= card.level; i++)
                        expInfo += "\n<color=#ECE28C>" + card.name + " достигает уровн€ " + i + "</color>";
                }
                card.addedExp = 0;
            }
            saveData.squad.gold += addedGold;
            expInfo += "\nќтр€д получил золота: " + addedGold.ToString();
            expInfo += "<color=#8E1F1F>" + losses + "</color>";
            endFightPanel.ShowVictoryWindow(expInfo);
            sounder.PlayOneShot(victory);
            GameObject.Find("Main Camera").GetComponent<AudioSource>().clip = null;
            return;
        }
    }
    public CardFightPrefab GetCardSlot(Card card)
    {
        foreach (var slot in playerCardPrefabs)
        {
            if (slot.card == card)
                return slot;
        }

        foreach (var slot in enemyCardPrefabs)
        {
            if (slot.card == card)
                return slot;
        }
        return null;
    }
    public void UpdateDisplayCards()
    {
        for (int i = 0; i < playerCardPrefabs.Length; i++)   
            playerCardPrefabs[i].UpdateDisplay();
        for (int i = 0; i < enemyCardPrefabs.Length; i++)
            enemyCardPrefabs[i].UpdateDisplay();
    }
    public void AddToLog(string text)
    {
        logText.text += ("\n-"+text);
        scrollRect.normalizedPosition = new Vector2(0, 0);
        Canvas.ForceUpdateCanvases(); 
        scrollRect.verticalNormalizedPosition = 0f; 
    }

    public void TapEndFightButton()
    {
        if(fightStatus == FightStatus.Victory)
        {
            BinaryFormatter formatter = new BinaryFormatter();
            saveData.currentFight = null;
            using (FileStream fs = new FileStream(loadPath, FileMode.OpenOrCreate))
            {
                formatter.Serialize(fs, saveData);
            }
            scenChanger.StartOpenScen("Game");
        }
        else if (fightStatus == FightStatus.Lose)
        {
            if (File.Exists(loadPath))
                File.Delete(loadPath);
            scenChanger.StartOpenScen("Menu");
        }
    }
    public void LoadFight()
    {
        BinaryFormatter formatter = new BinaryFormatter();

        string settingsPath = PlayerPrefs.GetString("savePath") + @"\settings.settings";

        using (FileStream fs = new FileStream(settingsPath, FileMode.OpenOrCreate))
        {
            SettingsData settings = (SettingsData)formatter.Deserialize(fs);
            GameObject.Find("SoundManager").GetComponent<LerpSound>().LerpTo(settings.soundVolume / 100f, 1.5f);
            GameObject.Find("Main Camera").GetComponent<LerpSound>().LerpTo((settings.musicVolume / 100f) * 0.75f, 2f);

        }
        using (FileStream fs = new FileStream(loadPath, FileMode.OpenOrCreate))
        {
            if (fs.Length <= 0)
            {
                fs.Close();
                File.Delete(loadPath);
                return;
            }
            SaveData save  = (SaveData)formatter.Deserialize(fs);
            saveData = save;
            FightData data = save.currentFight;
            this.data = data;
            if(data.musicID == 1)
            {
                GameObject.Find("Main Camera").GetComponent<AudioSource>().clip = bossFightTheme;
                GameObject.Find("Main Camera").GetComponent<AudioSource>().Play();
            }
           
        }
    }
    
}

public enum AttackResult
{
    SuccessfulAttack,
    Evasion,
    CriticalAttack
}

[System.Serializable]
public class FightData
{
    public Card[] playerCards = new Card[10];
    public Card[] enemyCards = new Card[10];

    public int musicID;
    public FightData(List<Card> player, List<Card> enemy, int musicID = 0)
    {
        playerCards = DistributedCards(player, new List<Class> { Class.Archer, Class.Healer });

        enemyCards = DistributedCards(enemy, new List<Class> { Class.Archer, Class.Healer, Class.OutlawArcher, Class.Lich });
        this.musicID = musicID;
    }

    public static Card[] DistributedCards(List<Card> cards, List<Class> backLineTypes)
    {
        Card[] result = new Card[10];
        List<Card> firstLine = new List<Card>();
        List<Card> secondLine = new List<Card>();

        foreach (Card card in cards)
        {
            if ((backLineTypes.Contains(card.type)) && secondLine.Count < 5)
                secondLine.Add(card);
            else if (firstLine.Count < 5)
                firstLine.Add(card);
            else if ((backLineTypes.Contains(card.type) == false) && secondLine.Count < 5)
                secondLine.Add(card);
            else
                firstLine.Add(card);
        }

        for (int i = 0; i < firstLine.Count; i++)
            result[i] = firstLine[i];

        for (int i = 0; i < secondLine.Count; i++)
            result[i + 5] = secondLine[i];
        
        return result;
    }
}
class EndFightPanel
{
    public TextMeshProUGUI title;
    public TextMeshProUGUI description;
    public TextMeshProUGUI button;
    public GameObject panel;

    public EndFightPanel(Transform panel)
    {
        this.panel = panel.gameObject;
        title = panel.GetChild(0).GetChild(0).GetComponent<TextMeshProUGUI>();
        description = panel.GetChild(0).GetChild(1).GetComponent<TextMeshProUGUI>();
        button = panel.GetChild(0).GetChild(2).GetChild(0).GetComponent<TextMeshProUGUI>();
    }

    public void ShowGameOver()
    {
        title.text = "ѕоражение";
        description.text = "¬аш отр€д отважно пал в бою. ¬ы погибли вместе со всеми, сража€сь";
        button.text = "¬ернутьс€ в меню";
        panel.SetActive(true);
    }
    public void ShowVictoryWindow(string expUpInfo)
    {
        title.text = "ѕобеда!";
        description.text = "¬раг пал. ¬аш отр€д одержал славную победу. \n " + expUpInfo;
        button.text = "ѕродолжить";
        panel.SetActive(true);
    }

}

public enum FightStatus
{
    Active,
    Lose,
    Victory
}