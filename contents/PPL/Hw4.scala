package hw4

import scala.collection.immutable.HashMap 
import hw4._


package object hw4 {
  type Env = HashMap[Var,LocVal]
}

case class Mem(m: HashMap[LocVal,Val], top: Int) {
  def extended(v: Val): (Mem, LocVal) = {
    val new_mem = Mem(m.updated(LocVal(top),v), top+1)
    (new_mem,LocVal(top))
  }
  def updated(l: LocVal, new_val: Val): Option[Mem] = {
    m.get(l) match {
      case Some(v) => Some(Mem(m.updated(l, new_val), top))
      case None => None
    }
  }
  def get(l: LocVal): Option[Val] = m.get(l)
  def getLocs(): List[LocVal] = m.keySet.toList
}

sealed trait Val
case object SkipVal extends Val
case class IntVal(n: Int) extends Val
case class BoolVal(b: Boolean) extends Val
case class ProcVal(args: List[Var], expr: Expr, env: Env) extends Val
case class LocVal(l: Int) extends Val
sealed trait RecordValLike extends Val
case object EmptyRecordVal extends RecordValLike
case class RecordVal(field: Var, loc: LocVal, next: RecordValLike) extends RecordValLike


sealed trait Program
sealed trait Expr extends Program
case object Skip extends Expr
case object False extends Expr
case object True extends Expr
case class NotExpr(expr: Expr) extends Expr
case class Const(n: Int) extends Expr
case class Var(s: String) extends Expr {
  override def toString = s"Var(${"\""}${s}${"\""})"
}
case class Add(l: Expr, r: Expr) extends Expr
case class Sub(l: Expr, r: Expr) extends Expr
case class Mul(l: Expr, r: Expr) extends Expr
case class Div(l: Expr, r: Expr) extends Expr
case class LTEExpr(l: Expr, r: Expr) extends Expr
case class EQExpr(l: Expr, r: Expr) extends Expr
case class Iszero(c: Expr) extends Expr
case class Ite(c: Expr, t: Expr, f: Expr) extends Expr
case class Let(i: Var, v: Expr, body: Expr) extends Expr
case class Proc(args: List[Var], expr: Expr) extends Expr
case class Asn(v: Var, e: Expr) extends Expr
case class BeginEnd(expr: Expr) extends Expr
case class FieldAccess(record: Expr, field: Var) extends Expr
case class FieldAssign(record: Expr, field: Var, new_val: Expr) extends Expr
case class Block(f: Expr, s: Expr) extends Expr
case class PCallV(ftn: Expr, arg: List[Expr]) extends Expr
case class PCallR(ftn: Expr, arg: List[Var]) extends Expr
case class WhileExpr(cond: Expr, body: Expr) extends Expr
sealed trait RecordLike extends Expr
case object EmptyRecordExpr extends RecordLike
case class RecordExpr(field: Var, initVal: Expr, next: RecordLike) extends RecordLike








object MiniCInterpreter {

  case class Result(v: Val, m: Mem)
  case class EM(enV: Env, meM: Mem)
  case class UndefinedSemantics(msg: String = "", cause: Throwable = None.orNull) extends Exception("Undefined Semantics: " ++ msg, cause)
    
  def Co_Access(record_v: RecordValLike, field: Var, record_m: Mem): Result = record_v match{
    case EmptyRecordVal => throw new UndefinedSemantics(s"message ${record_v}")
    case RecordVal(f, l, n) => {
      if(f.s == field.s){
        record_m.get(l) match{
          case Some(v) => Result(v, record_m)
          case None => throw new UndefinedSemantics(s"message ${record_m}")
        }
      }
      else{
        Co_Access(n, field, record_m)
      }
    }
  }

  def Co_Assign(record_v: RecordValLike, field: Var, record_m: Mem, new_v: Val): Result = record_v match{
    case EmptyRecordVal => throw new UndefinedSemantics(s"message ${record_v}")
    case RecordVal(f, l, n) => {
      if(f.s == field.s){
        record_m.updated(l, new_v) match{
          case Some(mm) => Result(new_v, mm)
          case None => throw new UndefinedSemantics(s"message ${record_m}")
        }
      }
      else{
        Co_Assign(n, field, record_m, new_v)
      }
    }
  }

  def Co_PCallV(proc: ProcVal, arg: List[Expr], e: Env, m: Mem):Result = arg match{
    case List() => {
      eval(proc.env, m, proc.expr)
    }
    case _ => eval(e, m, arg.head) match{
      case Result(value1, mem1) => {
        mem1.extended(value1) match{
          case (mem3, new_top) =>{
            val env2 = proc.env + (proc.args.head -> new_top)
            Co_PCallV(ProcVal(proc.args.tail, proc.expr, env2), arg.tail, e, mem3)
          }
          case _ => throw new UndefinedSemantics(s"message ${mem1}") 
        }
      }
      case _ => throw new UndefinedSemantics(s"message ${arg}") 
    }
  }

  def eval(env: Env, mem: Mem, expr: Expr): Result = expr match{
    case Skip => Result(SkipVal, mem)
    case False => Result(BoolVal(false), mem)
    case True => Result(BoolVal(true), mem)
    case NotExpr(expr) => eval(env, mem, expr) match{
      case Result(BoolVal(true), mem1) => Result(BoolVal(false), mem1)
      case Result(BoolVal(false), mem1) => Result(BoolVal(true), mem1)
      case _ => throw new UndefinedSemantics(s"message ${expr}")
    }
    case Const(n) => Result(IntVal(n), mem)
    case Var(s) => {
      if(env.exists((a: (Var, LocVal)) => a._1 == Var(s))){
        mem.get(env(Var(s))) match{
          case Some(n) => Result(n, mem)
          case None => throw new UndefinedSemantics(s"message20 ${s}")
        }
      }
      else{
        throw new UndefinedSemantics(s"message21 ${s}")
      }
    }
    case Add(l, r) => eval(env, mem, l) match{
      case Result(IntVal(n1), mem1) => eval(env, mem1, r) match{
        case Result(IntVal(n2), mem2) => Result(IntVal(n1+n2), mem2)
        case _ => throw new UndefinedSemantics(s"message ${r}")
      }
      case _ => throw new UndefinedSemantics(s"message ${l}")
    }
    case Sub(l, r) => eval(env, mem, l) match{
      case Result(IntVal(n1), mem1) => eval(env, mem1, r) match{
        case Result(IntVal(n2), mem2) => Result(IntVal(n1-n2), mem2)
        case _ => throw new UndefinedSemantics(s"message ${r}")
      }
      case _ => throw new UndefinedSemantics(s"message ${l}")
    }
    case Mul(l, r) => eval(env, mem, l) match{
      case Result(IntVal(n1), mem1) => eval(env, mem1, r) match{
        case Result(IntVal(n2), mem2) => Result(IntVal(n1*n2), mem2)
        case _ => throw new UndefinedSemantics(s"message ${r}")
      }
      case _ => throw new UndefinedSemantics(s"message ${l}")
    }
    case Div(l, r) => eval(env, mem, r) match{
      case Result(IntVal(n1), mem1) => {
        if(n1 == 0){
          throw new UndefinedSemantics(s"message ${r}")
        }
        else{
          eval(env, mem1, l) match{
            case Result(IntVal(n2), mem2) => Result(IntVal(n2/n1), mem2)
            case _ => throw new UndefinedSemantics(s"message ${l}")
          }
        }
      }
      case _ => throw new UndefinedSemantics(s"message ${r}")
    }
    case LTEExpr(l, r) => eval(env, mem, l) match{
      case Result(IntVal(n1), mem1) => eval(env, mem1, r) match{
        case Result(IntVal(n2), mem2) => {
          if(n1 <= n2) {
            Result(BoolVal(true), mem2)
          }
          else{
            Result(BoolVal(false), mem2)
          }
        }
        case _ => throw new UndefinedSemantics(s"message ${r}")
      }
      case _ => throw new UndefinedSemantics(s"message ${l}")
    }
    case EQExpr(l, r) => eval(env, mem, l) match{
      case Result(IntVal(n1), mem1) => eval(env, mem1, r) match{
        case Result(IntVal(n2), mem2) => {
          if(n1 == n2) {
            Result(BoolVal(true), mem2)
          }
          else{
            Result(BoolVal(false), mem2)
          }
        }
        case Result(BoolVal(b), mem2) => Result(BoolVal(false), mem2)
        case Result(SkipVal, mem2) => Result(BoolVal(false), mem2)
        case _ => throw new UndefinedSemantics(s"message ${r}")
      }
      case Result(BoolVal(n1), mem1) => eval(env, mem1, r) match{
        case Result(BoolVal(n2), mem2) => {
          if(n1 == n2) {
            Result(BoolVal(true), mem2)
          }
          else{
            Result(BoolVal(false), mem2)
          }
        }
        case Result(IntVal(n), mem2) => Result(BoolVal(false), mem2)
        case Result(SkipVal, mem2) => Result(BoolVal(false), mem2)
        case _ => throw new UndefinedSemantics(s"message ${r}")
      }
      case Result(SkipVal, mem1) => eval(env, mem1, r) match{
        case Result(SkipVal, mem2) => {
          Result(BoolVal(true), mem2)
        }
        case Result(IntVal(n), mem2) => Result(BoolVal(false), mem2)
        case Result(BoolVal(b), mem2) => Result(BoolVal(false), mem2)
        case _ => throw new UndefinedSemantics(s"message ${r}")
      }
      case _ => throw new UndefinedSemantics(s"message ${l}")
    }
    case Iszero(c) => eval(env, mem, c) match{
      case Result(IntVal(n), mem1) => {
        if(n==0){
          Result(BoolVal(true), mem1)
        }
        else{
          Result(BoolVal(false), mem1)
        }
      }
      case _ => throw new UndefinedSemantics(s"message ${c}")
    }
    case Ite(c, t, f) => eval(env, mem, c) match{
      case Result(BoolVal(true), mem1) => {
        eval(env, mem1, t)
      }
      case Result(BoolVal(false), mem1) => {
        eval(env, mem1, f)
      }
      case _ => throw new UndefinedSemantics(s"message ${c}")
    }
    case Let(i, v, body) => eval(env, mem, v) match{
      case Result(value, mem1) => {
        mem1.extended(value) match{
          case (mem2, new_top) =>{
            val env2 = env + (i -> new_top)
            eval(env2, mem2, body)
          }
          case _ => throw new UndefinedSemantics(s"message ${mem1}") 
        }
      }
      case _ => throw new UndefinedSemantics(s"message ${v}") 
    }
    case Proc(args, expr) => Result(ProcVal(args, expr, env), mem)
    case Asn(v, e) => eval(env, mem, e) match{
      case Result(value, mem1) => {
        if(env.exists((a: (Var, LocVal)) => a._1 == v)){
          mem1.updated(env(v), value) match{
            case Some(mem2) => Result(value, mem2)
            case _ => throw new UndefinedSemantics(s"message ${mem}")
          }
        }
        else{
          throw new UndefinedSemantics(s"message ${v}")
        }
      }
      case _ => throw new UndefinedSemantics(s"message ${e}")
    }
    case BeginEnd(expr) => eval(env, mem, expr) match{
      case Result(value, mem1) => Result(value, mem1)
      case _ => throw new UndefinedSemantics(s"message ${expr}")
    }
    case FieldAccess(record, field) => eval(env, mem, record) match{
      case Result(RecordVal(f, l, n), mem1) => {
        Co_Access(RecordVal(f, l, n), field, mem1)
      }
      case _ => throw new UndefinedSemantics(s"message ${record}")
    }
      
    case FieldAssign(record, field, new_val) => eval(env, mem, record) match{
      case Result(RecordVal(f, l, n), mem1) => {
        eval(env, mem1, new_val) match{
          case Result(v, mem2) => Co_Assign(RecordVal(f, l, n), field, mem2, v)
          case _ => throw new UndefinedSemantics(s"message ${new_val}")
        }
      }
      case _ => throw new UndefinedSemantics(s"message ${record}")
    }
    case Block(f, s) => eval(env, mem, f) match{
      case Result(value, mem1) =>{
        eval(env, mem1, s)
      }
      case _ => throw new UndefinedSemantics(s"message ${f}")
    }
    case PCallV(ftn, arg) => eval(env, mem, ftn) match{
      case Result(ProcVal(vars, expr, p_env), mem0) => Co_PCallV(ProcVal(vars, expr, p_env), arg, env, mem0)
      case _ => throw new UndefinedSemantics(s"message ${ftn}")
    }
    
    case PCallR(ftn, arg) => eval(env, mem, ftn) match{
      case Result(ProcVal(args, expr, p_env), mem1) => arg match{
        case Nil => eval(p_env, mem1, expr)
        case _ => {
          if(env.exists((a: (Var, Val)) => a._1 == arg.head)){
          val env2 = env + (args.head -> env(arg.head))
          eval(env2, mem1, PCallR(Proc(args.tail, expr), arg.tail))
          }
          else{
            throw new UndefinedSemantics(s"message10 ${arg.head}")
          }
        }
      }
      case _ => throw new UndefinedSemantics(s"message ${ftn}")
    }
    case WhileExpr(cond, body) => eval(env, mem, cond) match{
      case Result(BoolVal(true), mem0) => {
        eval(env, mem0, body) match{
          case Result(value, mem1) =>{
            eval(env, mem1, WhileExpr(cond, body)) match{
              case Result(value2, mem2) => Result(SkipVal, mem2)
            }
          }
        }
      }
      case Result(BoolVal(false), mem1) => Result(SkipVal, mem1)
      case _ => throw new UndefinedSemantics(s"message ${cond}")
    }
    case EmptyRecordExpr => Result(EmptyRecordVal, mem)
    case RecordExpr(field, initVal, next) => eval(env, mem, initVal) match{
      case Result(v, m1) => {
        m1.extended(v) match{
          case (mem3, new_top) => {
            next match{
              case RecordExpr(f, i, n) => {
                eval(env, mem3, RecordExpr(f, i, n)) match{
                  case Result(rv: RecordVal, mem4) => Result(RecordVal(field, new_top, rv), mem4)
                  case _ => throw new UndefinedSemantics(s"message ${next}") 
                }
              } 
              case EmptyRecordExpr => {
                Result(RecordVal(field, new_top, EmptyRecordVal), mem3)
              }
            }
          }
          case _ => throw new UndefinedSemantics(s"message ${m1}") 
        }
      }
    }
  }

  def rv_l(rec: RecordValLike, des: List[Int]): List[Int] = rec match{
    case EmptyRecordVal => des
    case RecordVal(f, l, n) => l.l::des
  }

  def addLoc(stg: List[Int], m: HashMap[LocVal, Val], des: HashMap[LocVal, Val]): HashMap[LocVal, Val] = stg match{
    case Nil => des
    case _ => {
      if(m.contains(LocVal(stg.head))){
        addLoc(stg.tail, m, des+(LocVal(stg.head) -> m.apply(LocVal(stg.head))))
      }
      else addLoc(stg.tail, m, des)
    }
  }

  def reachable(stg: List[Int], m: HashMap[LocVal, Val], sp: LocVal):List[Int] = {
    if(stg.contains(sp.l)) {
      List()
    }
    else{
      m.apply(sp) match{
        case LocVal(n) => {
          reachable(n::stg, m, LocVal(n))
        }
        case RecordVal(f, l, n) => {
          val r_stg = rv_l(RecordVal(f, l, n), List())
          sp.l::stg.concat(r_stg)
        }
        case _ => {
          sp.l::stg
        }
      }
    }
  }
    

  def Co_gc(env_l: List[LocVal], init_m: HashMap[LocVal, Val], result_m: HashMap[LocVal, Val]): HashMap[LocVal, Val] = env_l match{
    case List() => result_m
    case _ => {
      init_m.apply(env_l.head) match{
        case LocVal(n) => {
          val ss = reachable(List(), init_m, LocVal(n))
          if(ss != Nil){
            val result_m2= Co_gc(env_l.tail, init_m, addLoc(ss, init_m, result_m))
            Co_gc(env_l.tail, init_m, result_m2 + (env_l.head -> init_m.apply(env_l.head)))
          }
          else{
            Co_gc(env_l.tail, init_m, result_m)
          }
        }
        case _ => Co_gc(env_l.tail, init_m, result_m + (env_l.head -> init_m.apply(env_l.head)))
      }
    }
  }

  def gc(env: Env, mem: Mem): Mem = {
    val env_l = env.values.toList
    val empty_m = HashMap[LocVal,Val]()
    val new_m = Co_gc(env_l, mem.m, empty_m)
    Mem(new_m, mem.top)
  }
  
  def apply(program: String): (Val, Mem) = {
    val parsed = MiniCParserDriver(program)
    val res = eval(new Env(), Mem(new HashMap[LocVal,Val],0), parsed)
    (res.v, res.m)
  }

}


object Hw4App extends App {
  
  println("Hello from Hw4!")

}